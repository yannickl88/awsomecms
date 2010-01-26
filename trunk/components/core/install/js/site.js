function deleteMultiple(url, key, checkboxes)
{
    var querystring = "";
    
    checkboxes.each(function(i, value) {
        if($(value).attr("checked"))
        {
            querystring += "&"+key+"[]="+$(value).val();
        }
    });
    
    document.location = url+querystring;
}

function addFieldError(field, message)
{
    $(document).ready(function(e) {
        $('#'+field).parent().parent().addClass("form_error");
        if(message != '')
        {
            $('#errorlist').append("<li><label for='"+field+"'>"+$('label[for="'+field+'"]').html()+" "+message+"</label></li>");
        }
    });
}