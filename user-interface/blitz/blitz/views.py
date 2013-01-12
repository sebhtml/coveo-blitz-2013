from pyramid.view import view_config
import requests
import json

_endpoint='http://ec2-50-17-15-66.compute-1.amazonaws.com:11111/Korafle.cgi/search'

_data = 'http://ec2-204-236-255-208.compute-1.amazonaws.com:8080'
_images = _data+'/BlitzDataWebService/images/'


_tmp = '{"facets": {"origin": {"Georgia": 1,"White Plains": 1},"type": {"albums": 32,"artists": 19},"genres": {"Alternative rock": 2,"Pop rock": 1,"Indie rock": 1}},"results": [{"id": "011_x9"} ]}'


@view_config(route_name='home', renderer='templates/mytemplate.pt')
def my_view(request):
    
    return {'project': 'blitz'}

@view_config(route_name='search', renderer='templates/pascal.pt')
def main_search(request):
    s = request.GET['search']
    r = requests.get(_endpoint, params={'q':s})
    #elem = json.loads(r.content)
    D = json.loads(_tmp)
    
    
    FACES = ''
    if D.has_key('facets'):
        for section, fval in D['facets'].iteritems():
            #rint '*********', section , '*********'
            FACES += '<h5 class="section">' + section + '</h5>'
            FACES += '<ul>'
            
            for nom,  nb in fval.iteritems():
                FACES += '<li> ' + nom + ' (' + str(nb) + ')  </li>'
                #print nom
                #print nb
                
            FACES += '</ul>'
            
    RESULTS = '<ul class="thumbnails">'
    if D.has_key('results'):
        for item in D['results']:
            name = '...'
            text = '...'
            if item.has_key('id') : 
                id = item['id']
                name = str(id)
                img_url = _images + str(id)
            if item.has_key('text') : name = str(item['text'])
            
            
            
            RESULTS += """<li class="span3">
                          <figure>
                              <img src="%(img_url)s" alt="" />
                              <figcaption>
                                  <h5>%(name)s</h5>
                                  <p>%(text)s <a href="#"
                                                                    class="tooltips"
                                                                    data-placement="top"
                                                                    data-trigger="hover"
                                                                    data-title="HTML: Hyper Text Markup Language"
                                              ></a></p>
                              </figcaption>
                          </figure>
                      </li>""" %  {'name':name, 'text':text, 'img_url':img_url}           
                      
    RESULTS +=  '</ul>'
        
            
    return {'faces':FACES, 'results':RESULTS}