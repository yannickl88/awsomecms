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

var admin_treeTarget = "";
/**
 * Check the classes from an element that contain an ID
 * 
 * @param Array array
 * @return String		or False on failure
 */
function admin_fetchIDFromClasses(array)
{
	for(var key in array)
	{
		var value = array[key];
		if(value.substr(0, 4) === "link")
		{
			return value.substr(4);
		}
	}
	return false;
}
/**
 * Create a Cookie
 * 
 * @param String name
 * @param String value
 * @param Date days
 * @return Boolean		False
 */
function admin_createCookie(name,value,days) {
    if (days) 
    {
        var date = new Date();
        date.setTime(date.getTime()+(days*24*60*60*1000));
        var expires = "; expires="+date.toGMTString();
    }
    else 
        var expires = "";
    
    document.cookie = name+"="+value+expires+"; path=/";
    
    return false;
}
/**
 * Read a Cookie
 * 
 * @param String name
 * @return String 		or False on failure
 */
function admin_readCookie(name) {
    var nameEQ = name + "=";
    var ca = document.cookie.split(';');
    for(var i=0;i < ca.length;i++) 
    {
        var c = ca[i];
        
        while (c.charAt(0)==' ') 
            c = c.substring(1,c.length);
        
        if (c.indexOf(nameEQ) == 0) 
            return c.substring(nameEQ.length,c.length);
    }
    return false;
}
/**
 * Remove a cookie
 * 
 * @param String name
 * @return Boolean		False
 */
function admin_eraseCookie(name) {
    admin_createCookie(name,"",-1);
    
    return false;
}
/**
 * Check if there is a cookie
 * 
 * @param String name
 * @return Boolean
 */
function admin_hasCookie(name) {
    return (admin_readCookie(name));
}
/**
 * Mouse over listener
 * 
 * @param Event event
 */
function admin_mouseOver(event)
{
	$(this).addClass('focus')
}
/**
 * Mouse out listener
 * 
 * @param Event event
 */
function admin_mouseOut(event)
{
	$(this).removeClass('focus')
}
/**
 * Mouse click listener
 * 
 * @param Event event
 * @return Boolean		False
 */
function admin_mouseClick(event)
{
	var element = $(this);
	
	if($(this).hasClass("clickableTreeItem") && $(this).hasClass("javascript"))
	{
		$("#"+admin_treeTarget).val($(this).attr("href"));
		return false;
	}
	else if($(this).hasClass("clickableTreeItem"))
	{
		element = $(this).siblings(".clickableTreeArrow");
	}
	
	element.toggleClass('open');
	element.siblings(".treeNode").toggleClass('hidden');
	
	var id = admin_fetchIDFromClasses(element.attr("class").split(" "))
	
    if(admin_hasCookie('menu'+id))
    {
        admin_eraseCookie('menu'+id);
    }
    else
    {
        admin_createCookie('menu'+id, 1);
    }
	
    return false;
}

//some listeners for the clickeable items
$(document).ready(function(e) {
	$(".clickableTreeArrow").mouseover(admin_mouseOver);
	$(".clickableTreeArrow").mouseout(admin_mouseOut);
	$(".clickableTreeArrow").click(admin_mouseClick);
	$(".clickableTreeItem").click(admin_mouseClick);
});