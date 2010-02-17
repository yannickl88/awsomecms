function admin_toggleTree(id)
{
    $('.link'+id).toggleClass('open');
    $('.sub'+id).toggleClass('hidden');
    
    if(admin_hasCookie('menu'+id))
    {
        admin_eraseCookie('menu'+id);
    }
    else
    {
        admin_createCookie('menu'+id, true);
    }
    
    return false;
}

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

function admin_eraseCookie(name) {
    admin_createCookie(name,"",-1);
    
    return false;
}

function admin_hasCookie(name) {
    return (admin_readCookie(name) != null);
}