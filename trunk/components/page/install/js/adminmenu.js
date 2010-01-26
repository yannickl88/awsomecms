var adminmenuTimer = false;

function adminmenu_openMenu(item)
{
    var children = $(item).children("ul");
    var offset = $(item).offset();
    
    adminmenuTimer = false;
    
    $(item).siblings().children('li ul').each(function(key, value) {
        adminmenu_hideItem($(value).parent());
    });
    
    $(item).children("a").addClass('open');
    children.css('display', 'block');
    children.css('left', $(item).width() - 1);
    children.css('top', 0);
}

function adminmenu_hideItem(item)
{
    var children = $(item).children("ul");
    
    children.css('display', 'none');
    children.find('li').each(function(key, value) {
        adminmenu_hideItem(value);
    });
}

function adminmenu_closeMenu(item)
{
    adminmenuTimer = true;
    
    $(item).children("a").removeClass('open');
    
    setTimeout(function(){
        if(adminmenuTimer)
        {
            adminmenu_hideItem(item);
        }
    }, 1000);
}