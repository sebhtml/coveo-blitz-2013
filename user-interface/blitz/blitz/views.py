from pyramid.view import view_config
import requests
import json

_endpoint='http://ec2-50-17-15-66.compute-1.amazonaws.com:11111/Korafle.cgi/search'

_data = 'http://ec2-204-236-255-208.compute-1.amazonaws.com:8080'
_images = _data+'/BlitzDataWebService/images/'


#_tmp = '{"facets": {"origin": {"Georgia": 1,"White Plains": 1},"type": {"albums": 32,"artists": 19},"genres": {"Alternative rock": 2,"Pop rock": 1,"Indie rock": 1}},"results": [{"id": "011_x9"} ]}'
_tmp = '{"facets":{"FACET_0_NAME": {"KEY_0_NAME":10,"KEY_1_NAME":10,"KEY_2_NAME":10,"KEY_3_NAME":10},"FACET_1_NAME": {"KEY_0_NAME":10,"KEY_1_NAME":10,"KEY_2_NAME":10,"KEY_3_NAME":10},"FACET_2_NAME": {"KEY_0_NAME":10,"KEY_1_NAME":10,"KEY_2_NAME":10,"KEY_3_NAME":10}}, "results": [ {"id":0,"text":"Document description"},{"id":1,"text":"Document description"},{"id":2,"text":"Document description"},{"id":3,"text":"Document description"},{"id":4,"text":"Document description"},{"id":5,"text":"Document description"},{"id":6,"text":"Document description"},{"id":7,"text":"Document description"},{"id":8,"text":"Document description"},{"id":9,"text":"Document description"},{"id":10,"text":"Document description"},{"id":11,"text":"Document description"},{"id":12,"text":"Document description"},{"id":13,"text":"Document description"},{"id":14,"text":"Document description"}]} '


@view_config(route_name='home', renderer='templates/mytemplate.pt')
def my_view(request):
    
    return {'project': 'blitz'}

@view_config(route_name='search', renderer='templates/pascal2.pt')
def main_search(request):
    s = request.GET['search']
    r = requests.get(_endpoint, params={'q':s})
    #elem = json.loads(r.content)
    D = json.loads(_tmp)
    
    
    FACES = ''
    if D.has_key('facets'):
        i = 0
        for section, fval in D['facets'].iteritems():
            
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
                FACES += '<li> ' + nom + ' (' + str(nb) + ')  </li>'
                #print nom
                #print nb
                
            FACES += '</ul></div></div></div>'
            
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
                                  <h5>%(name)s</h5>
                                  <p>%(text)s</p>

                              </div>



                          </div>

                      </li>""" %  {'name':name, 'text':text, 'img_url':img_url}           
                      
    RESULTS +=  '</ul>'
        
            
    return {'faces':FACES, 'results':RESULTS}