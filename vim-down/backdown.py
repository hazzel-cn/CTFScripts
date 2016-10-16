# encoding=utf8 
import os
import sys
import time
import requests
import urlparse
from bs4 import BeautifulSoup


class checkUrl(object):
    """docstring for checkDown"""
    def __init__(self, url):
        if url[0:4] != 'http':
            url = 'http://' + url
        self.__url = url
        self.__filename = os.path.basename(url)
        self.__dirname = os.path.split(url)
        self.__script = os.path.splitext(url)

    def vimdown(self, suffix):
        u = os.path.join(self.__dirname[0], '.' + self.__filename + suffix)
        print '[Try]', u,
        if requests.get(u).status_code != 404:
            print '[Success]'
            os.system('wget ' + u + ' -O ./pages/' + self.__filename + suffix)
        else:
            print '[Fail]'


    def wave(self): # For gedit
        u = self.__url + '~'
        print '[Try]', u,
        if requests.get(u).status_code != 404:
            print '[Success]'
            os.system('wget ' + u + ' -O ./pages/' + self.__filename + '~')
        else:
            print '[Fail]'


def spyder_run(url):

    if url[0:4] != 'http':
        url = 'http://' + url

    urls = [url]
    visited = [url]

    txt = open('./urllist.txt', 'w+')

    while len(urls) > 0:
        sys.stdout.flush()
        htmltext = requests.get(urls[0]).text

        txt.writelines(urls[0] + '\n')

        soup = BeautifulSoup(htmltext, "html.parser")
        print len(urls), urls[0]
        urls.pop(0)

        for tag in soup.findAll('a', href = True):
            tag['href'] = urlparse.urljoin(url, tag['href'])
            if url in tag['href'] and tag['href'] not in visited and "#" not in tag['href']: # While in the sanme domin
                urls.append(tag['href'])
                visited.append(tag['href'])

    txt.close()



def main():
    spyder = raw_input('Spyder or not?(Beta)[Y/n]:')

    if spyder == 'n':
        pass
    else:
        initUrl = raw_input('Input URL:')
        spyder_run(initUrl)

    try:
        os.mkdir('pages')
    except:
        pass
    #Please ensure no blanks at EOF
    urls = []
    for line in open("urllist.txt"): 
        u = line.replace("\n", "")
        print "[*] Loading", u
        urls.append(u)
    print "[*] Load complete."

    while True:
        for url in urls:
            cu = checkUrl(url)
            try:
                cu.vimdown('.swp')
                cu.vimdown('.swn')
                cu.vimdown('.swo')
                cu.vimdown('.swp4')
                cu.wave()
            except:
                pass
        time.sleep(3)

if __name__ == '__main__':
    main()
