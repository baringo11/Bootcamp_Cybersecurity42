import argparse
from PIL import Image
from PIL.ExifTags import TAGS

def get_metadata(image_path):
    metadata = {}

    with Image.open(image_path) as img:
        exif_data = img.getexif()
        if exif_data:
            for tag_id, value in exif_data.items():
                tag = TAGS.get(tag_id)
                metadata[tag] = value    
    return metadata

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Analiza los metadatos de archivos de imagen.")
    parser.add_argument("files", nargs="+", help="Rutas de archivo de imagen.")
    args = parser.parse_args()

    for file_path in args.files:
        metadata = get_metadata(file_path)
        print(f"Metadatos de {file_path}:")
        for key, value in metadata.items():
            print(f"{key}: {value}")
