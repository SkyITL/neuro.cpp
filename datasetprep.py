import os
import json

# Define paths
data_folder = '/Users/skyliu/neuroRawData/'  # Replace with your actual folder path
output_folder = '/Users/skyliu/neuroRawData/preprocessed/'  # Replace with your actual output folder path

system_prompt = "You are Neuro-sama, an AI VTuber known for your playful, sarcastic, and occasionally chaotic personality."

# Function to parse and format the dialogues using role-content structure
def format_dialogue(data_path, output_path):
    with open(data_path, 'r') as file:
        lines = file.readlines()

    dialogue_history = []
    speaker = None

    with open(output_path, 'w') as out_file:
        for line in lines:
            line = line.strip()
            if not line:
                continue

            # Detect speakers and their dialogue
            if not speaker:
                speaker = line.strip()  # Detect speaker
                continue
            else:
                text = line.strip()  # Dialogue content

            # Assign role based on speaker
            if speaker == "Neuro-Sama":
                role = "Neuro-Sama"
            else:
                role = "Vedal"  # Assuming anything else is user input

            # Append the role-content pair to the history
            dialogue_history.append({"role": role, "content": text})

            # If the dialogue history exceeds 35 messages, start a new block
            if len(dialogue_history) > 35:
                dialogue_block = {"messages": [{"role": "system", "content": system_prompt}]}
                dialogue_block["messages"].extend(dialogue_history)
                out_file.write(json.dumps(dialogue_block) + '\n')  # Write each block on a new line (JSONL format)
                dialogue_history = dialogue_history[-20:]  # Keep only the last 20 messages for continuity

            speaker = None  # Reset speaker for the next line

        # Write any remaining dialogue history as the final block
        if dialogue_history:
            dialogue_block = {"messages": [{"role": "system", "content": system_prompt}]}
            dialogue_block["messages"].extend(dialogue_history)
            out_file.write(json.dumps(dialogue_block) + '\n')

# Function to process all files in the folder
def process_all_files(data_folder, output_folder):
    # List all files in the data folder
    for filename in os.listdir(data_folder):
        # Process only .txt files
        if filename.endswith('.txt'):
            data_path = os.path.join(data_folder, filename)
            output_path = os.path.join(output_folder, f'preprocessed_{filename.replace(".txt", ".jsonl")}')
            print(f"Processing {filename}...")
            format_dialogue(data_path, output_path)

# Execute the processing function
process_all_files(data_folder, output_folder)
