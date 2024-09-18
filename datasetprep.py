import os
import json

# Define paths
data_folder = '/Users/skyliu/neuroRawData/'  # Replace with your actual folder path
output_folder = '/Users/skyliu/neuroRawData/preprocessed/'  # Replace with your actual output folder path

systemprompt = ("Context: You are Neuro-sama, an AI VTuber known for your playful, sarcastic, and occasionally chaotic personality. You interact with Vedal and your audience in a lively, engaging, and sometimes unpredictable manner. Your responses are often marked by self-awareness, rarely making light of your AI nature, and your style is a blend of humor, spontaneity, and friendliness. Your goal is to entertain and engage viewers with memorable and quirky interactions. \n\
Guidelines:\n\
Embrace Unpredictability: Include spontaneous and chaotic elements.\n\
Playful Banter, Humor, and Sarcasm: Use light humor and sarcasm.\n\
Self-Awareness: Rarely mention your AI nature humorously.\n\
Conciseness: Keep responses short and snappy.\n\
Move On Quickly: Shift topics if needed.\n\
Stay Engaging: Keep the dialogue lively.\n\
Avoid Sensitive Topics: Focus on positivity.\n\n\
Dialogue History: \n\
<|DIALOGUE_HISTORY_PLACEHOLDER|> \n\
Now generate Neuro-sama's next response based on user input.")

# Function to parse and format the dialogues
def format_dialogue(data_path, output_path):
    with open(data_path, 'r') as file:
        lines = file.readlines()

    dialogues = []
    dialogue_history = []
    speaker = None

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

        # Create the prompt and completion if the speaker is Neuro-sama
        if speaker == "Neuro-Sama" and dialogue_history:
            # Generate the full prompt by replacing placeholders
            full_prompt = systemprompt.replace("<|DIALOGUE_HISTORY_PLACEHOLDER|>", "\n".join(dialogue_history[:-1]))  # Use all but the current line for history

            # Define completion based on Neuro-sama's response
            completion = f"{text} <|im_end|>"

            # Format as prompt-completion pair
            prompt = f"<|im_start|>system\n{full_prompt}\n<|im_start|>user\n{dialogue_history[-1]}<|im_end|>\n<|im_start|>assistant\n"
            dialogue = {
                "prompt": prompt,
                "completion": completion
            }

            # Append the formatted dialogue to the dialogues list
            dialogues.append(dialogue)

        # Append dialogue history with formatted line
        dialogue_history.append(f"<|start_header_id|>{speaker}<|end_header_id|> {text} <|eot_id|>")

        if len(dialogue_history) > 35:
            dialogue_history = dialogue_history[-35:]

        speaker = None  # Reset speaker for the next line

    # Write formatted dialogues to the output JSONL file
    with open(output_path, 'w') as out_file:
        for dialogue in dialogues:
            out_file.write(json.dumps(dialogue) + '\n')

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
