Avser Huvudfil som kör hela flödet

example.
from vision import capture_image
from inference import run_inference
from interface import send_results_to_api
from utils import load_config

def main():
    config = load_config()
    image = capture_image(config["camera"]["device_id"], config["camera"]["resolution"])
    results = run_inference(image, config["model"]["path"], config["model"]["confidence_threshold"])
    
    if config["api"]["enabled"]:
        send_results_to_api(results, config["api"]["endpoint"], config["api"]["token"])

if __name__ == "__main__":
    main()

