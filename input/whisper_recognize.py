import sys
import numpy as np
import torch
from whisper import load_model

def transcribe_audio_file(file_path):
    # Load the Whisper model
    model = load_model("base")

    # Read the audio data from the file
    with open(file_path, 'rb') as f:
        audio_data = f.read()

    # Convert the bytes to a numpy array of floats
    audio_array = np.frombuffer(audio_data, dtype=np.float32)

    # Ensure the array is writable by copying it
    audio_array = np.copy(audio_array)  # Make sure array is writable

    # Proceed with transcription
    result = model.transcribe(audio_array, fp16=False)  # Set fp16=False for CPU compatibility

    # Print the transcribed result
    print(result['text'])

if __name__ == "__main__":
    # Pass the path to the audio file as a command-line argument
    if len(sys.argv) > 1:
        transcribe_audio_file(sys.argv[1])
    else:
        print("Usage: python3 whisper_recognize.py <path_to_audio_file>")
