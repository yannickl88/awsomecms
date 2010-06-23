/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package js
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

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