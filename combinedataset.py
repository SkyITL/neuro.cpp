import os
import json
import random

# Define paths
input_folder = '/Users/skyliu/neuroRawData/secondary'  # Update with your folder path containing JSONL files
combined_output_path = '/Users/skyliu/Documents/Llama3/Neuro_Data/V4/secondary_dataset/combined.jsonl'  # Path for the combined JSONL file
train_output_path = '/Users/skyliu/Documents/Llama3/Neuro_Data/V4/secondary_dataset/train.jsonl'  # Path for the training set JSONL file
valid_output_path = '/Users/skyliu/Documents/Llama3/Neuro_Data/V4/secondary_dataset/valid.jsonl'  # Path for the validation set JSONL file

# Combine JSONL files
def combine_jsonl_files(input_folder, combined_output_path):
    combined_data = []
    
    # Iterate through all files in the directory
    for filename in os.listdir(input_folder):
        if filename.endswith('.jsonl'):
            file_path = os.path.join(input_folder, filename)
            with open(file_path, 'r') as file:
                for line in file:
                    combined_data.append(json.loads(line))

    # Write combined data to a new JSONL file
    with open(combined_output_path, 'w') as outfile:
        for entry in combined_data:
            outfile.write(json.dumps(entry) + '\n')

    return combined_data

# Split data into training and validation sets
def split_data(data, train_ratio=0.8):
    random.shuffle(data)  # Shuffle the data to ensure randomness
    split_index = int(len(data) * train_ratio)
    train_data = data[:split_index]
    valid_data = data[split_index:]
    return train_data, valid_data

# Save data to JSONL file
def save_jsonl(data, output_path):
    with open(output_path, 'w') as outfile:
        for entry in data:
            outfile.write(json.dumps(entry) + '\n')

# Main execution
if __name__ == "__main__":
    # Combine all JSONL files
    combined_data = combine_jsonl_files(input_folder, combined_output_path)

    # Split combined data into train and validation sets
    train_data, valid_data = split_data(combined_data)

    # Save the train and validation datasets
    save_jsonl(train_data, train_output_path)
    save_jsonl(valid_data, valid_output_path)

    print(f"Combined data saved to: {combined_output_path}")
    print(f"Training data saved to: {train_output_path}")
    print(f"Validation data saved to: {valid_output_path}")