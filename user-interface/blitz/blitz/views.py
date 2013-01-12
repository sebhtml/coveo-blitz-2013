from pyramid.view import view_config
import requests
import json

_endpoint='http://ec2-50-17-15-66.compute-1.amazonaws.com:11111/Korafle.cgi/search'

_data = 'http://ec2-204-236-255-208.compute-1.amazonaws.com:8080'
_images = _data+'/BlitzDataWebService/images/'


#_tmp = '{"facets": {"origin": {"Georgia": 1,"White Plains": 1},"type": {"albums": 32,"artists": 19},"genres": {"Alternative rock": 2,"Pop rock": 1,"Indie rock": 1}},"results": [{"id": "011_x9"} ]}'
_tmp = '{"facets":{"FACET_0_NAME": {"KEY_0_NAME":10,"KEY_1_NAME":10,"KEY_2_NAME":10,"KEY_3_NAME":10},"FACET_1_NAME": {"KEY_0_NAME":10,"KEY_1_NAME":10,"KEY_2_NAME":10,"KEY_3_NAME":10},"FACET_2_NAME": {"KEY_0_NAME":10,"KEY_1_NAME":10,"KEY_2_NAME":10,"KEY_3_NAME":10}}, "results": [ {"id":"011_x9","text":"Album name"},{"id":\"011_x9\","text":"Album name"},{"id":2,"text":"Album name"},{"id":3,"text":"Album name"},{"id":4,"text":"Album name"},{"id":5,"text":"Album name"},{"id":6,"text":"Album name"},{"id":7,"text":"Album name"},{"id":8,"text":"Album name"},{"id":9,"text":"Album name"},{"id":10,"text":"Album name"},{"id":11,"text":"Album name"},{"id":12,"text":"Album name"},{"id":13,"text":"Album name"},{"id":14,"text":"Album name"}]} '

_SESSIONS = dict()

@view_config(route_name='home', renderer='templates/mytemplate.pt')
def my_view(request):
    
    return {'project': 'blitz'}

def gen_facets(D):
    FACES = ''
    i = 0
    for section, fval in D.iteritems():
            
        #rint '*********', section , '*********'
        i += 1
        FACES += """<div class="accordion"> <div class="accordion-heading">
                              <a class="accordion-toggle" data-parent="#accordion""" + str(i) + """" data-toggle="collapse" href="#panel""" + str(i) + """"><i class="icon-chevron-right"></i>"""
        FACES += section
        FACES += """</a>
                    </div>
                    <div class="accordion-body collapse" id="panel""" + str(i) + """">
                            <div class="accordion-inner">"""
        FACES += '<ul>'
            
        for nom,  nb in fval.iteritems():
            FACES += '<li> ' + "<a href=\"#\">" +  nom + ' (' + str(nb) + ')  </li>'
            #print nom
            #print nb
                
        FACES += '</ul></div></div></div>'
    return FACES
 
def gen_results(L):
    RESULTS = '<ul class="thumbnails">'

    for item in L:
        name = '...'
        text = 'album description'
        if item.has_key('id') : 
            id = item['id']
            name = str(id)
            img_url = _images + str(id)
        if item.has_key('text') : name = str(item['text'])
            
        RESULTS += """                      <li class="span9">
                          <div class="row">
                              <div class="span3">
                                  <figure >
                                      <div style="margin-top: 0px;">
                                          <img src="%(img_url)s" width="200" style="margin-top:0;" />
                                      </div>
                                  </figure>
                              </div>

                              <div class="span6" style="margin-top:20px; margin-left: -15px;">
                                  <h3>%(name)s</h3>
                                  <h6>%(text)s</h6>

                              </div>



                          </div>

            </li>""" %  {'name':name, 'text':text, 'img_url':img_url}           
                      
    RESULTS +=  '</ul>'
    return RESULTS
                    

@view_config(route_name='search', renderer='templates/pascal2.pt')
def main_search(request):
    s = request.GET['search']
    session = request.session
    session['last'] = s
    
    r = requests.get(_endpoint, params={'q':s})
    #elem = json.loads(r.content)
    D = json.loads(_tmp)
    
    if D.has_key('facets'):
        FACES = gen_facets(D['facets'])

    if D.has_key('results'):
        RESULTS = gen_results(D['results'])

            
    return {'faces':FACES, 'results':RESULTS}

@view_config(route_name='rafine', renderer='string')
def rafine_search(request):
    print 'rafine!'
    session = request.session
    if 'last' not in session: session['last'] = ''
    
    
    s = session['last'] + '+' + request.GET['search']
        
    print request.GET['search']
    r = requests.get(_endpoint, params={'q':s})
    #elem = json.loads(r.content)
    D = json.loads(_tmp)
    
    if D.has_key('results'):
        RESULTS = gen_results(D['results'])
        
    return RESULTS
        
    

    