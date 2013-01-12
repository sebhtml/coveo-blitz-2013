__author__ = 'Alexandre'
import requests
import json
from multiprocessing import Process, Pool
from time import time

data_service = 'http://ec2-204-236-255-208.compute-1.amazonaws.com:8080/BlitzDataWebService/'
max_retries = 10
timeout = 15

def get_artist(id):
    for i in xrange(max_retries):
        try:
            request = requests.get(data_service + 'artists/' + str(id), timeout=timeout)
            break;
        except:
            pass
    data = dict(json.loads(request.content))
    data['type'] = u'artist'
    data = trim_ends(data)
    index_data(data)
    #print data['name']

def get_album(id):
    for i in xrange(max_retries):
        try:
            request = requests.get(data_service + 'albums/' + str(id), timeout=timeout)
            break;
        except:
            pass
    data = dict(json.loads(request.content))
    data['type'] = u'album'
    data = trim_ends(data)
    index_data(data)
    #print data['name']

def crawl_artists():
    page_size = 100

    for i in xrange(max_retries):
        try:
            request = requests.get(data_service + 'artists?size=' + str(page_size) + '&page=0', timeout=timeout)
            break;
        except:
            pass

    data = dict(json.loads(request.content))

    if not data.has_key('totalPages'):
        n_pages = 0
    else:
        n_pages = int(data['totalPages'])

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
            workers.apply_async(get_artist, args=(artist['id'],))


def crawl_albums():
    page_size = 100

    for i in xrange(max_retries):
        try:
            request = requests.get(data_service + 'albums?size=' + str(page_size) + '&page=0', timeout=timeout)
            break;
        except:
            pass

    data = dict(json.loads(request.content))

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
            workers.apply_async(get_album, args=(album['id'],))


def index_data(data):
    requests.post('http://ec2-50-17-15-66.compute-1.amazonaws.com:11111/Korafle.cgi/push', data=json.dumps(data))

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

def start_run(run_id):
    print requests.get(data_service + 'evaluationRun/start', params={'runId':run_id}).url

def stop_run():
    requests.get(data_service + 'evaluationRun/stop')

if __name__ == "__main__":
    t = time()
    workers = Pool(50)
    start_run('Run1test')
    crawl_artists()
    crawl_albums()
    workers.close()
    workers.join()
    stop_run()
    print "Took: ", time()-t