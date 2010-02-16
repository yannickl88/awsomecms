/**
 * Function for redirecting to a delete url for multiple records
 * 
 * @param string url
 * @param string key
 * @param DOMElement checkboxes
 */
function deleteMultiple(url, key, checkboxes)
{
    var querystring = "";
    
    checkboxes.each(function(i, value) {
        if($(value).attr("checked"))
        {
            if(querystring != "")
            {
              querystring += "&";
            }
          
            querystring += key+"[]="+$(value).val();
        }
    });
    
    document.location = url+"?"+querystring;
}
/**
 * Mark a field that it had an error
 * 
 * @param DOMElement field
 * @param string message
 */
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
/**
 * Call a component using GET
 * 
 * @param string component
 * @param string action
 * @param object data
 * @param function onsucces
 */
function getComponent(component, action, data, onsucces)
{
    if(!data)
    {
        data = {};
    }
    if(!onsucces)
    {
        onsucces = function(data) {};
    }
    
    data.action = action;
    data.component = component;
    
    $.get("/", data, onsucces);
}
/**
 * Call a component using POST
 * 
 * @param string component
 * @param string action
 * @param object data
 * @param function onsucces
 */
function postComponent(component, action, data, onsucces)
{
    if(!data)
    {
        data = {};
    }
    if(!onsucces)
    {
        onsucces = function(data) {};
    }
    
    data.action = action;
    data.component = component;
    
    $.post("/", data, onsucces);
}