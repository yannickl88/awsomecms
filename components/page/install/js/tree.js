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
 * Admin tree with nodes
 */
var Tree = function(element, javascript)
{
    this.children = new Array();
    this.contentType = 1;
    this.iconMap = {};
    this.html = null;
    this.javascript = (javascript)? javascript : false;
    this.foldersOnly = false;

    /**
     * Constructor
     */
    this.init = function(element)
    {
        this.html = $("<div class='treeNode'></div>");
        if(!element || element == "")
        {
            var id = "treePlaceholder_"+Math.floor(Math.random()*1000);
            document.write("<div id='"+id+"'></div>");
            $("#"+id).replaceWith(this.html);
        }
        else
        {
            $("#"+element).append(this.html);
        }
    };
    /**
     * Add a child to the folder
     * 
     * @param TreeItem child
     * @param Array locArray
     */
    this.addChild = function(child)
    {
        if (child.location == "/")
        {
            var x = 0;
            while(x < this.children.length)
            {
                if(this.children[x].name > child.name && this.children[x].type <= child.type)
                {
                    break;
                }
                x++;
            }
            if(x == this.children.length)
            {
                if(!this.foldersOnly || child.type != Tree.PAGE)
                {
                    this.html.append(child.html);
                }
                this.children.push(child);
            }
            else
            {
                if(!this.foldersOnly || child.type != Tree.PAGE)
                {
                    this.children[x].html.before(child.html);
                }
                this.children.splice(x, 0, child);
            }
            child.setParent(this);
        }
        else
        {
            var locArray = child.location.substr(1, child.location.length-2).split("/");
            var loc = locArray.shift();
            var inserted = false;

            for (var key in this.children)
            {
                var node = this.children[key];

                if (node.type == Tree.PAGE_FOLDER && node.getPath() == "/" + loc)
                {
                    inserted = node.addChild(child, locArray);
                    break;
                }
            }

            if (!inserted)
            {
                var folder = new TreeFolder(loc, "/", this, "folder");
                var x = 0;
                while(x < this.children.length)
                {
                    if(this.children[x].name > folder.name && this.children[x].type <= folder.type)
                    {
                        break;
                    }
                    x++;
                }
                if(x == this.children.length)
                {
                    this.children.push(folder);
                    this.html.children(".treeNode").append(folder.html);
                }
                else
                {
                    this.children[x].html.before(folder.html)
                    this.children.splice(x, 0, folder);
                }
                folder.setParent(this);
                folder.addChild(child, locArray);
                this.html.append(folder.html);
            }
        }
    };
    /**
     * Add an icon to the collection
     * 
     * @param String key
     * @param String icon   URL to the icon
     */
    this.assignIcon = function(key, icon) 
    {
        this.iconMap[key] = icon;
    };
    /**
     * Get an icon with a certain key
     * 
     * @param String key
     * @return String URL to the icon
     */
    this.getIcon = function(key) 
    {
        return this.iconMap[key];
    };
    
    this.getPath = function()
    {
        return "/";
    };
    this.getLocPath = function()
    {
        return "/";
    };
    this.open = function()
    {
        //this does nothing, but we need it to solve the recursion
        return;
    };

    this.init(element);
}
// Constants
Tree.PAGE = 1;
Tree.PAGE_FOLDER = 2;

/**
 * Folder page item
 */
var TreeItem = function(name, location, url, root, icon, actions, contentType)
{
    this.name = name;
    this.location = location;
    this.parent = null;
    this.url = url;
    this.type = Tree.PAGE;
    this.contentType = contentType;
    this.html = null;
    this.root = root;
    this.actions = actions;
    this.icon = $("<img alt='page' src='"+this.root.getIcon(icon)+"' />");
    this.link = null;
    
    /**
     * Constructor
     */
    this.init = function()
    {
        this.html = $("<div class='hideIcons'></div>");
        this.html[0].object = this;
        this.link = $("<a href='"+this.url+"'>"+this.name+"</a>");
        this.link.prepend(this.icon);
        this.html.append(this.link);
        if(this.actions)
        {
            for(var x in this.actions)
            {
                this.addAction(this.actions[x]);
            }
        }
        this.html.mouseover(function(e){
            $(this).removeClass("hideIcons");
        });
        this.html.mouseout(function(e){
            $(this).addClass("hideIcons");
        });
    };
    this.addAction = function(action)
    {
        this.html.prepend(action.html);
    }
    this.setParent = function(parent)
    {
        this.parent = parent;
        if(!this.contentType)
        {
            this.contentType = parent.contentType;
        }
    };
    
    this.init();
}
/**
 * Folder item for a tree
 */
var TreeFolder = function(name, location, root, icon, actions, contentType, isRootFolder)
{
    this.name = name;
    this.location = location;
    this.children = new Array();
    this.parent = null;
    this.type = Tree.PAGE_FOLDER;
    this.contentType = contentType;
    this.html = null;
    this.openFolder = false;
    this.root = root;
    this.actions = actions;
    this.icon = icon;
    this.isRootFolder = isRootFolder;
    
    /**
     * Constructor
     */
    this.init = function()
    {
        var ref = this;
        this.html = $("<div class='hideIcons'></div>");
        this.html[0].object = ref;
        if(this.actions)
        {
            var x = this.actions.length - 1;
            while(x >= 0)
            {
                this.html.append(this.actions[x].html);
                x--;
            }
        }
        this.html.append("<a class='folder clickableTreeArrow' href='#'></a>");
        this.html.append("<img src='"+this.root.getIcon(this.icon)+"' alt='folder'/>");
        this.html.append("<a class='clickableTreeItem' style='padding: 0pt;' href='#'>"+this.name+"</a>");
        this.html.append("<div class='treeNode hidden'></div>");
        this.html.children(".clickableTreeArrow").bind({
            mouseover: function(e) {
                $(this).addClass("focus");
            },
            mouseout: function(e) {
                $(this).removeClass("focus");
            },
            click: function(e) {
                if(ref.openFolder)
                {
                    ref.close();
                }
                else
                {
                    ref.open();
                }
                e.preventDefault();
                return false;
            }
        });
        this.html.children(".clickableTreeItem").click(function(e) {
            if(ref.root.javascript)
            {
                $("#"+ref.root.javascript).val(ref.getLocPath());
            }
            else
            {
                if(ref.openFolder)
                {
                    ref.close();
                }
                else
                {
                    ref.open();
                }
            }
            e.preventDefault();
            return false;
        });
        if(!this.root.foldersOnly)
        {
            this.html.mouseover(function(e){
                $(this).removeClass("hideIcons");
            });
            this.html.mouseout(function(e){
                $(this).addClass("hideIcons");
            });
        }
        
        //open when we have a cookie
        if(this.hasCookie(this.getPath()))
        {
            this.open();
        }
    };
    this.setParent = function(parent)
    {
        this.parent = parent;
        if(!this.contentType)
        {
            this.contentType = parent.contentType;
        }
    };
    /**
     * Add a child to the folder
     * 
     * @param TreeItem child
     * @param Array locArray
     * @return success
     */
    this.addChild = function(child, locArray)
    {
        if(locArray.length == 0)
        {
            var x = 0;
            while(x < this.children.length)
            {
                if(this.children[x].name > child.name && this.children[x].type <= child.type)
                {
                    break;
                }
                x++;
            }
            if(x == this.children.length)
            {
                if(!this.root.foldersOnly || child.type != Tree.PAGE)
                {
                    this.html.children(".treeNode").append(child.html);
                }
                this.children.push(child);
            }
            else
            {
                if(!this.root.foldersOnly || child.type != Tree.PAGE)
                {
                    this.children[x].html.before(child.html);
                }
                this.children.splice(x, 0, child);
            }
            child.setParent(this);
            return true;
        }
        
        var loc = locArray.shift();
        var inserted = false;

        for (var key in this.children)
        {
            var node = this.children[key];
            
            if (node.type == Tree.PAGE_FOLDER && node.getPath() == this.getPath() + "/" + loc)
            {
                node.addChild(child, locArray);
                inserted = true;
                break;
            }
        }
        
        if (!inserted)
        {
            var folder = new TreeFolder(loc, this.getPath()+"/", this.root, "folder");
            var x = 0;
            while(x < this.children.length)
            {
                if(this.children[x].name > folder.name && this.children[x].type <= folder.type)
                {
                    break;
                }
                x++;
            }
            if(x == this.children.length)
            {
                this.children.push(folder);
                this.html.children(".treeNode").append(folder.html);
            }
            else
            {
                this.children[x].html.before(folder.html)
                this.children.splice(x, 0, folder);
            }
            folder.setParent(this);
            return folder.addChild(child, locArray);
        }
        
        return inserted;
    };
    this.removeChild = function(node)
    {
        node.html.remove();
        this.children.splice(this.children.indexOf(node), 1);
    };
    /**
     * Get the tree path of this folder
     * 
     * @return String
     */
    this.getPath = function()
    {
        return this.location+this.name;
    };
    /**
     * Get the location path of this folder
     * 
     * @return String
     */
    this.getLocPath = function()
    {
        if(this.isRootFolder)
        {
            return this.parent.getLocPath();
        }
        return this.parent.getLocPath()+this.name+"/";
    };
    /**
     * Open the folder
     */
    this.open = function() 
    {
        this.html.children(".treeNode").removeClass("hidden");
        this.html.children(".clickableTreeArrow").addClass("open");
        this.openFolder = true;
        
        this.createCookie(this.getPath(), 1);
        
        if(this.parent)
        {
            this.parent.open();
        }
    };
    /**
     * Collapse the folder
     */
    this.close = function()
    {
        this.html.children(".treeNode").addClass("hidden");
        this.html.children(".clickableTreeArrow").removeClass("open");
        this.openFolder = false;
        
        this.eraseCookie(this.getPath());
    };
    /**
     * Create a Cookie
     * 
     * @param String name
     * @param String value
     * @param Date days
     * @return Boolean      False
     */
    this.createCookie = function(name, value, days) {
        if (days) 
        {
            var date = new Date();
            date.setTime(date.getTime()+(days*24*60*60*1000));
            var expires = "; expires="+date.toGMTString();
        }
        else 
            var expires = "";

        document.cookie = name+"="+value+expires+"; path=/";
    };
    /**
     * Read a Cookie
     * 
     * @param String name
     * @return String       or False on failure
     */
    this.readCookie = function(name) {
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
    };
    /**
     * Remove a cookie
     * 
     * @param String name
     * @return Boolean      False
     */
    this.eraseCookie = function(name) {
        this.createCookie(name,"",-1);
    };
    /**
     * Check if there is a cookie
     * 
     * @param String name
     * @return Boolean
     */
    this.hasCookie = function(name) {
        return (this.readCookie(name));
    };
    
    this.init();
}
/**
 * Actions for a tree node, these are icons displayed to the right when hovered over the node
 * 
 * @param String icon
 * @param String url
 */
var TreeAction = function(icon, url, root)
{
    this.icon = icon;
    this.url = url;
    this.html = null;
    this.root = root;
    
    this.init = function()
    {
        this.html = $("<a class='treeIcon' href='"+this.url+"'><img src='"+this.root.getIcon(this.icon)+"' alt='tree action'/></a>");
    }
    
    this.init();
}