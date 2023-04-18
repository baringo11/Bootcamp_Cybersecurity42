from bs4 import BeautifulSoup
from urllib.parse import urlparse, urljoin
import argparse
import os
import requests

urls = {}

def spider(url, dpath, recursive = False, max_depth = 5, current_depth = 0):
    extensions = [".jpg", ".jpeg", ".png", ".gif", ".bmp"]
    parsed_url = urlparse(url)
    base_url = parsed_url.scheme + "://" + parsed_url.netloc

    print(f"\n*** depth: {current_depth} URL: {url} ***\n")

    os.makedirs(dpath, exist_ok=True)

    try:
        response = requests.get(url)
        htmlDoc = BeautifulSoup(response.content, "html.parser")
    except:
        print("Error getting the url")
        return

    urls[url] = 1

    for img in htmlDoc.find_all("img"):
        img_src = img.get("src")
        if os.path.splitext(img_src)[1] in extensions:
            img_url = urljoin(url, img_src)
            if urls.get(img_url) is None:
                print(img_url)
                urls[img_url] = 1
                try:
                    with open(os.path.join(dpath, os.path.basename(img_src)), "wb") as f:
                        f.write(requests.get(img_url).content)
                except:
                    print("Error downloading image")
                    continue
    print("\n-----------\n")
    if recursive and current_depth < max_depth:
        for link in htmlDoc.find_all("a"):
            print(link)
            href = link.get("href")
            if href and not href.startswith('#'):
                print(link)
                full_url = urljoin(base_url, href)
                if urls.get(full_url) is None:
                   return spider(full_url, dpath, recursive, max_depth, current_depth+1)

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("url", type=str, help="URL del sitio web.")
    parser.add_argument("-r", action="store_true", help="Descarga de forma recursiva.")
    parser.add_argument("-l", type=int, default=5, help="Nivel de profundidad máximo de la descarga recursiva.")
    parser.add_argument("-p", type=str, default="./data/", help="Ruta donde se guardarán los archivos descargados.")
    args = parser.parse_args()

    spider(args.url, args.p, args.r, args.l)
