__author__ = 'Alexandre'
import requests
import json
from multiprocessing import Process, Pool
from time import time

data_service = 'http://ec2-204-236-255-208.compute-1.amazonaws.com:8080/BlitzDataWebService/'


def start_run(run_id):
    print requests.get(data_service + 'evaluationRun/start', params={'runId':run_id}).url

def stop_run():
    requests.get(data_service + 'evaluationRun/stop')

def get_artist(id):
    #print 'Hello worker ARTIST'
    request = requests.get(data_service + 'artists/' + str(id))
    data = dict(json.loads(request.content))
    data['type'] = u'artist'
    data = trim_ends(data)
    index_data(data)
    print data['name']

def get_album(id):
    #print 'Hello worker ALBUM'
    request = requests.get(data_service + 'albums/' + str(id))
    data = dict(json.loads(request.content))
    data['type'] = u'album'
    data = trim_ends(data)
    index_data(data)
    print data['name']

def crawl_artists():
    page_size = 100
    data = dict(json.loads(requests.get(data_service + 'artists?size=' + str(page_size) + '&page=0').content))

    if not data.has_key('totalPages'):
        n_pages = 0
    else:
        n_pages = int(data['totalPages'])
    print n_pages

    # Get artists list for all pages
    for i in xrange(n_pages):
        # Reuse first query
        if i != 0:
            data = dict(json.loads(requests.get(data_service + 'artists?size=' + str(page_size) + '&page=0' + str(i)).content))
            #print data

        # Artists on page i
        artists = list(data['content'])

        for artist in artists:
            # Get artists data async
            workers.apply_async(get_artist, args=(artist['id'],), callback=hello)
            #count +=1
            #print 'worker submit'

def hello(test):
    print 'Hello!', test

def crawl_albums():
    page_size = 100
    data = dict(json.loads(requests.get(data_service + 'albums?size=' + str(page_size) + '&page=0').content))

    if not data.has_key('totalPages'):
        n_pages = 0
    else:
        n_pages = int(data['totalPages'])

    # Get artists list for all pages
    for i in xrange(n_pages):
        # Reuse first query
        if i != 0:
            data = dict(json.loads(requests.get(data_service + 'albums?size=' + str(page_size) + '&page=0' + str(i)).content))
            #print data

        # Artists on page i
        albums = list(data['content'])

        for album in albums:
            # Get artists data async
            workers.apply_async(get_album, args=(album['id'],), callback=hello)
            #count +=1

def callback_artist(request):
    pass

def callback_album(request):
    data = dict(json.loads(request.content))
    data['type'] = u'album'
    data = trim_ends(data)
    index_data(data)
    print data['name']

def index_data(data):
    requests.post('http://ec2-50-17-15-66.compute-1.amazonaws.com:11111/Korafle.cgi/push', data=data)
    #count += 1

def trim_ends(object):
    if unicode == type(object):
        return unicode(object).strip()
    elif list == type(object):
        new_list = []
        for o in object:
            new_list.append(trim_ends(o))
        return new_list
    elif dict == type(object):
        new_dict = {}
        for key in object.keys():
            new_dict[key] = trim_ends(object[key])
        return new_dict


if __name__ == "__main__":
    t = time()
    start_run('Run1test')
    #count = 0
    artists_crawler = Process(target=crawl_artists)
    albums_crawler = Process(target=crawl_albums)
    workers = Pool(50)

    crawl_artists()
    #crawl_artists()
    crawl_albums()
    #crawl_albums()



    #albums_crawler.start()
    #artists_crawler.start()

    #albums_crawler.join()
    #artists_crawler.join()

    workers.close()
    workers.join()

    print "Took: ", time()-t

    from time import sleep
    #sleep(10)