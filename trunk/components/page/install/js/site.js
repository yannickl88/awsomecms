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
    field = field.split("#");
    section = field[1];
    
    $('label[for='+field[0]+']').parents(".admin_form_row").addClass("form_error");
    
    if(message != '')
    {
        sectionStr = "";
        if(section) sectionStr = "("+section+")";
        $('#errorlist').append("<li><label for='"+field[0]+"'>"+sectionStr+$('label[for="'+field[0]+'"]').html()+" "+message+"</label></li>");
        
        $().ready(function(e) {
            if($('label[for='+field[0]+']').parents(".admin_form_row").data("toggleSection"))
                $('label[for='+field[0]+']').parents(".admin_form_row").data("toggleSection")(section);
        });
    }
    
    return false;
}
function setFieldValue(field, value)
{
    var field = $('form *[name='+field+']');
    value = Base64.decode(value);
    
    if(!field.hasClass("empty"))
    {
        if(field.data('setValue'))
            field.data('setValue').setValue(value);
        else {
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
    }
    
    return false;
}
