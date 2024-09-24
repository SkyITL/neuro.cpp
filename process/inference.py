import torch
from transformers import AutoTokenizer, AutoModelForSequenceClassification

# Set device to MPS for macOS GPU
device = torch.device("mps" if torch.backends.mps.is_available() else "cpu")

# Load DistilBERT model and tokenizer
model_name = "distilbert-base-uncased-finetuned-sst-2-english"
tokenizer = AutoTokenizer.from_pretrained(model_name)
model = AutoModelForSequenceClassification.from_pretrained(model_name).to(device)

# Example text input for sentiment analysis
text = "I love using my new Mac M3 for deep learning!"

# Tokenize the input
inputs = tokenizer(text, return_tensors="pt").to(device)

# Perform inference
outputs = model(**inputs)

# Get the predicted label
logits = outputs.logits
predicted_class = logits.argmax(dim=-1).item()

print(f"Predicted class: {predicted_class}")
