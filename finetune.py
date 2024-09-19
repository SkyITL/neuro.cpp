import torch
from transformers import AutoModelForCausalLM, AutoTokenizer, Trainer, TrainingArguments, DataCollatorWithPadding
from torch.utils.data import DataLoader, Dataset
from safetensors.torch import save_file
import os
import json

# Define a custom dataset class
class CustomDataset(Dataset):
    def __init__(self, data_path, tokenizer):
        self.tokenizer = tokenizer
        with open(data_path, 'r') as file:
            self.data = [json.loads(line) for line in file]

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        item = self.data[idx]
        inputs = self.tokenizer(item['prompt'], return_tensors='pt', truncation=False)  # No truncation
        targets = self.tokenizer(item['completion'], return_tensors='pt', truncation=False)

        input_ids = inputs['input_ids'].squeeze()
        labels = targets['input_ids'].squeeze()

        # Ensure labels align with input_ids
        labels = torch.cat([labels, torch.full((input_ids.size(0) - labels.size(0),), -100)])  # -100 will be ignored by the loss

        return {
            'input_ids': input_ids,
            'labels': labels
        }

# LoRA Adapter Setup
import torch.nn as nn

class LoRAAdapter(nn.Module):
    def __init__(self, input_dim, output_dim, r):
        """
        LoRA adapter: rank decomposition of a weight matrix.
        input_dim: dimension of the input
        output_dim: dimension of the output
        r: rank for LoRA
        """
        super(LoRAAdapter, self).__init__()
        # Define two small learned matrices for low-rank approximation
        self.lora_A = nn.Parameter(torch.randn(input_dim, r))
        self.lora_B = nn.Parameter(torch.randn(r, output_dim))
    
    def forward(self, x):
        # Apply low-rank adaptation by multiplying the input by the LoRA matrices
        return x @ self.lora_A @ self.lora_B

def apply_lora(model, lora_layers):
    """
    Apply LoRA adaptation to the attention layers.
    lora_layers: Number of LoRA layers to apply (rank).
    """
    for name, module in list(model.named_modules()):
        if 'q_proj' in name or 'v_proj' in name or 'k_proj' in name:  # Apply to attention projection layers
            # Freeze the original parameters
            for param in module.parameters():
                param.requires_grad = False
            
            # Get the shape of the original weight matrix
            input_dim, output_dim = module.weight.shape
            
            # Apply LoRA adapters with the desired rank (lora_layers)
            lora_adapter = LoRAAdapter(input_dim, output_dim, lora_layers)
            model.add_module(f'{name.replace(".", "_")}_lora', lora_adapter)

    return model


def train_lora(model, tokenizer, train_path, valid_path, output_dir, lora_layers, learning_rate=1e-4, epochs=1, batch_size=1):
    train_dataset = CustomDataset(train_path, tokenizer)
    valid_dataset = CustomDataset(valid_path, tokenizer)

    # Apply LoRA to the model
    model = apply_lora(model, lora_layers)

    # Define the training arguments
    training_args = TrainingArguments(
        output_dir=output_dir,
        overwrite_output_dir=True,
        num_train_epochs=epochs,
        per_device_train_batch_size=batch_size,
        save_steps=10,
        save_total_limit=2,
        logging_steps=2,
        learning_rate=learning_rate,
        weight_decay=0.01,
        logging_dir='./logs',
        report_to='none',
        prediction_loss_only=True,  # Focus on generation evaluation
        eval_steps=10,
        eval_strategy="steps"
    )
    
    # Make sure tokenizer has a pad token
    if tokenizer.pad_token is None:
        tokenizer.pad_token = tokenizer.eos_token

    # Use DataCollatorWithPadding to automatically pad input_ids and labels
    data_collator = DataCollatorWithPadding(tokenizer)

    # Initialize the Trainer
    trainer = Trainer(
        model=model,
        args=training_args,
        train_dataset=train_dataset,
        eval_dataset=valid_dataset,  # Use the separate validation dataset
        data_collator=lambda data: {'input_ids': torch.stack([f['input_ids'] for f in data]),
                                    'labels': torch.stack([f['labels'] for f in data])}
    )

    # Start training
    trainer.train()

    # Save adapters
    save_lora_adapters(model, output_dir)

def save_lora_adapters(model, output_dir):
    # Save only the LoRA adapter weights
    lora_state_dict = {k: v for k, v in model.state_dict().items() if 'lora' in k}
    adapter_path = os.path.join(output_dir, 'adapters.safetensors')
    save_file(lora_state_dict, adapter_path)
    print(f'Saved LoRA adapters to {adapter_path}')

# Main function to execute tuning
def run_lora_tuning(model_path, train_path, valid_path, output_dir, lora_layers):
    tokenizer = AutoTokenizer.from_pretrained(model_path)
    model = AutoModelForCausalLM.from_pretrained(model_path)

    train_lora(model, tokenizer, train_path, valid_path, output_dir, lora_layers)

# Example usage
if __name__ == '__main__':
    model_path = '/Users/skyliu/Downloads/llama3.1_8B'  # Replace with your safetensor model path
    train_path = '/Users/skyliu/Documents/Llama3/Neuro_Data/V4/train.jsonl'  # Replace with your training dataset path
    valid_path = '/Users/skyliu/Documents/Llama3/Neuro_Data/V4/valid.jsonl'  # Replace with your validation dataset path
    output_dir = '/Users/skyliu/Downloads/neuro/adapters'  # Directory where adapters will be saved
    lora_layers = 24  # Define how many layers you want to apply LoRA

    run_lora_tuning(model_path, train_path, valid_path, output_dir, lora_layers)
