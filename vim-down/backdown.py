# encoding=utf8 
import os
import sys
import time
import requests
import urlparse
from bs4 import BeautifulSoup


class checkUrl(object):
    def __init__(self, url):
        if url[0:4] != 'http':
            url = 'http://' + url
        self.__url = url
        #self.__filename = os.path.basename(url)
        self.__dirname = os.path.split(url)
        #self.__script = os.path.splitext(url)

    def vimdown(self, suffix):
        #u = os.path.join(self.__dirname[0], '/.' + self.__dirname[1] + suffix)
        u = self.__dirname[0] + '/.' + self.__dirname[1] + suffix
        print '\n[Try]', u

        ##### Doing GET request ####
        print ' *[ GET]',
        r = requests.get(u)
        if r.status_code == 200:
            print '[Success] <<<===========\a'
            f = open('./pages/' + self.__dirname[1] + suffix,'wb')
            f.write(r.content)
        else:
            print '[Fail]', r.status_code

        ##### Doing POST request ####
        print ' *[POST]',
        r = requests.post(u)
        if r.status_code == 200:
            print '[Success] <<<===========\a'
            f = open('./pages/' + self.__dirname[1] + suffix,'wb')
            f.write(r.content)
        else:
            print '[Fail]', r.status_code


    def wave(self): # For gedit
        u = self.__url + '~'
        print '\n[Try]', u

        ##### Doing GET request ####
        print ' *[ GET]',
        r = requests.get(u)
        if r.status_code == 200:
            print '[Success] <<<===========\a'
            f = open('./pages/' + self.__dirname[1] + suffix,'wb')
            f.write(r.content)
        else:
            print '[Fail]', r.status_code

        ##### Doing POST request ####
        print ' *[POST]',
        r = requests.post(u)
        if r.status_code == 200:
            print '[Success] <<<===========\a'
            f = open('./pages/' + self.__dirname[1] + suffix,'wb')
            f.write(r.content)
        else:
            print '[Fail]', r.status_code


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
    spyder = raw_input('Spyder or not?(Beta)[N/y]:')

    if spyder == 'y':
        initUrl = raw_input('Input URL:')
        spyder_run(initUrl)
    else:
        pass

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
                cu.vimdown(suffix='.swp')
                cu.vimdown(suffix='.swn')
                cu.vimdown(suffix='.swo')
                cu.vimdown(suffix='.swp4')
                cu.wave()
            except Exception,e:
                print Exception,":",e
        time.sleep(3)

if __name__ == '__main__':
    main()
