import sys
import whisper

def recognize(audio_path):
    model = whisper.load_model("base")  # Load the desired model size
    result = model.transcribe(audio_path)
    print(result["text"])  # Print the transcription

if __name__ == "__main__":
    audio_file = sys.argv[1]  # Get the audio file path from command line
    recognize(audio_file)