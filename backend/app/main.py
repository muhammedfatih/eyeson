from fastapi import FastAPI
from fastapi.responses import FileResponse, RedirectResponse
from fastapi.middleware.cors import CORSMiddleware
from configparser import ConfigParser
import os
from app.models.config import Config

app = FastAPI()
app.add_middleware(
   CORSMiddleware,
   allow_origins=["*"],
   allow_methods=["*"]
)

config = ConfigParser()
config_file = os.getenv("CONFIG_FILE")
if config_file is None:
    config_file = "/app/config/config.ini"

config.read(config_file)
output_folder = config["NEARMISS"]["output"]
last_frame_path = config["BACKEND"]["lastframepath"]

def get_coco_names():
    coco_names = []
    coco_name_file_path = config["DETECTOR"]["names"]
    with open(coco_name_file_path, "r") as coco_name_file:
        for line in coco_name_file:
            coco_names.append(line.strip())
    return coco_names

@app.get("/configs")
def get_configs():
    visibleConfigKeys = []
    for key in config.options("WEB_INTERFACE_TYPES"):
        visibleConfigKeys.append(key)

    configs = {}
    for section in config.sections():
        if section.startswith("WEB_INTERFACE_"):
            continue
        configs[section] = {}
        configs[section]["keys"] = []
        for key in config.options(section):
            if key in visibleConfigKeys:
                keyValue = config[section][key]
                keyType = config["WEB_INTERFACE_TYPES"][key]
                keyLabel = config["WEB_INTERFACE_LABELS"][key]
                sectionKeyValue = {
                    "key": key,
                    "value": keyValue,
                    "type": keyType,
                    "label": keyLabel
                }
                if keyType == "coconames":
                    sectionKeyValue["options"] = get_coco_names()

                configs[section]["keys"].append(sectionKeyValue)
                configs[section]["label"] = config["WEB_INTERFACE_LABELS"][section] 

        if len(configs[section]["keys"]) == 0:
            del configs[section]
            
    return configs

@app.post("/config/{section}/{key}")
def set_config_section_key(section: str, key: str, updatedConfig: Config):
    config.set(section, key, updatedConfig.value)
    with open(config_file, "w") as configfile:
        config.write(configfile)
    return config[section][key]

@app.get("/thumbnails")
def get_thumbnails():
    thumbnails = []
    for filename in os.listdir(output_folder):
        if filename.endswith("_small.jpg"):
            thumbnails.append(f"{filename.replace('_small.jpg', '')}")

    return thumbnails

@app.get("/images/{filename}")
def get_image(filename: str):
    return FileResponse(f"{output_folder}{filename}_big.jpg")

@app.get("/thumbnails/{filename}")
def get_thumbnail(filename: str):
    return FileResponse(f"{output_folder}{filename}_small.jpg")

@app.get("/last_image")
def get_last_image():
    return FileResponse(last_frame_path)