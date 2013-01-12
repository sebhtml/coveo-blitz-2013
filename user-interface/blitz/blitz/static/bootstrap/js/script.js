/**
 * Created with PyCharm.
 * User: brice
 * Date: 12/01/13
 * Time: 4:09 PM
 * To change this template use File | Settings | File Templates.
 */
/**
 * Created with PyCharm.
 * User: brice
 * Date: 12/01/13
 * Time: 1:12 PM
 * To change this template use File | Settings | File Templates.
 */


$(function() {

    $( "#search").autocomplete({
        source: function( request, response ) {
            $.ajax({
                url: "/search/?search="+document.getElementById('search').value,
                dataType: "jsonp",
                data: {
                    featureClass: "P",
                    style: "full",
                    maxRows: 2,
                    name_startsWith: request.term
                },
                success: function( data ) {
                    alert(data);

                    response( $.map( data.results, function( item ) {
                        return {
                            label: item.text,
                            value: item.text
                        }
                    }));
                }
            });
        },
        minLength: 2,
        select: function( request1, response1 ) {
            $(function ()
            {
                function updates(){

                    $.ajax( {
                        url:'/updates.json?search='+document.getElementById('search').value,
                        dataType: "json",
                        success: function( data ) {

                            theresult ='';
                            // compteur = 0;
                            results = data.results;
                            $.each( results , function(item){

                                theresult += '<div class="line-separator"></div>';
                                theresult += '<div class="result-title"><a class="link-color" href="'+results[item].url+'">'+results[item].titleNoFormatting+ '</a></div>';
                                theresult += '<div class="result-link"> '+results[item].url+'</div>';
                                theresult += '<div class="result-snippet">'+results[item].content+'</div>';
                                //compteur = compteur +1;

                            } );

                            // $( "#result" ).html(theresult);


                        }


                    } );

                }

                updates();

            });





        },
        open: function() {
            $( this ).removeClass( "ui-corner-all" ).addClass( "ui-corner-top" );
        },
        close: function() {
            $( this ).removeClass( "ui-corner-top" ).addClass( "ui-corner-all" );
        }
    });
});



