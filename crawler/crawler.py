__author__ = 'Alexandre'
import requests
import json
from multiprocessing import Process, Pool
from itertools import count

data_service = 'http://ec2-204-236-255-208.compute-1.amazonaws.com:8080/BlitzDataWebService/'
count = 0

def start_run(run_id):
    requests.get(data_service + 'evaluationRun/start', params={'runId':run_id})

def stop_run():
    requests.get(data_service + 'evaluationRun/stop')

def crawl_artists():
    page_size = 100
    data = dict(json.loads(requests.get(data_service + 'artists?size=' + str(page_size) + '&page=0').content))

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
            requests.get(data_service + 'artists/' + artist['id'], hooks=dict(response=callback_artist))

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
        artists = list(data['content'])

        for artist in artists:
            # Get artists data async
            requests.get(data_service + 'albums/' + artist['id'], hooks=dict(response=callback_album))

def callback_artist(request):
    data = dict(json.loads(request.content))
    data['type'] = u'artist'
    data = trim_ends(data)
    index_data(data)
    print data['name']

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
    start_run('Peewee')

    artists_crawler = Process(target=crawl_artists)
    albums_crawler = Process(target=crawl_albums)

    albums_crawler.start()
    artists_crawler.start()

    albums_crawler.join()
    artists_crawler.join()

    stop_run()