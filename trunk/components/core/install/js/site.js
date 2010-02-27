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
    var count = 0;
    
    checkboxes.each(function(i, value) {
        if($(value).attr("checked"))
        {
            if(querystring != "")
            {
              querystring += "&";
            }
          
            querystring += key+"[]="+$(value).val();
            count++;
        }
    });
    
    if(count > 0)
    {
        document.location = url+"?"+querystring;
    }
    
    return false;
}
/**
 * Mark a field that it had an error
 * 
 * @param DOMElement field
 * @param string message
 */
function addFieldError(field, message)
{
    $('#'+field).parents(".admin_form_row").addClass("form_error");
    
    if(message != '')
    {
        $('#errorlist').append("<li><label for='"+field+"'>"+$('label[for="'+field+'"]').html()+" "+message+"</label></li>");
    }
    
    return false;
}
function setFieldValue(field, value)
{
    var field = $('form *[name='+field+']');
    
    if(!field.hasClass("empty"))
    {
        switch(field.attr("type"))
        {
            case "checkbox":
                field.attr("checked", (value == "on")? "checked": "");
                break;
            case "radio":
                field.each(function(key, inputField) {
                    $(inputField).attr("checked", (value == $(inputField).val())? "checked": "");
                });
                break;
            case "textarea":
                field.html(value);
                break;
            default:
                field.val(value);
        }
    }
    
    return false;
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
    
    var request = {
        action: action,
        component: component,
        data: data
    };
    
    $.get("/", request, onsucces, "json");
    
    return false;
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
    
    $.post("/", data, onsucces, "json");
    
    return false;
}

function openForm(component, action)
{
    getComponent("core", "getform", {component: component, form: action}, function(data) {
        $("#formWrapper").html(decodeURIComponent(data.html));
        $("#formWrapperBackground").css("display", "block");
    });
    return false;
}
function openTable(table, action)
{
    getComponent("core", "getform", {table: table, form: action}, function(data) {
        $("#formWrapper").html(decodeURIComponent(data.html));
        $("#formWrapperBackground").css("display", "block");
    });
    return false;
}