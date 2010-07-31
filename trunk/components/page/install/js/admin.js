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
 * Simple editor class which turns a textarea into a BBCode Text editor
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
var Editor = function(element, options)
{
    this.element = null;
    this.toolbar = null;
    /**
     * Constructor
     * 
     * @param DOMElement element
     * @param Object options
     */
    this.init = function(element, options)
    {
        this.element = element;
        
        if(this.element.parent().attr("class") != "toolbar")
        {
            this.element.wrap("<div class='toolbar' />");
            this.toolbar = $("<div class='toolbarButtons' />");
            this.element.parent().prepend(this.toolbar);
        }
        
        if(options != undefined)
        {
            if(options.width != undefined) 
                this.element.parent().width(options.width);
                this.element.width(options.width);
            if(options.height != undefined) 
                this.element.height(options.height);
        }
        //add items to the toolbar
        this._addButton("Bold", "[b]", "[/b]", 5);
        this._addButton("Italic", "[i]", "[/i]", 6);
        this._addButton("Underlined", "[u]", "[/u]", 7);
        this._addSpacer();
        this._addButton("Header 1", "[h1]", "[/h1]", 27);
        this._addButton("Header 2", "[h2]", "[/h2]", 28);
        this._addSpacer();
        this._addButton("Align Left", "[left]", "[/left]", 15);
        this._addButton("Align Center", "[center]", "[/center]", 16);
        this._addButton("Align Right", "[right]", "[/right]", 17);
        this._addSpacer();
        this._addButton("Hyperlink", "[url]", "[/url]", 23);
        this._addButton("E-mail", "[mail]", "[/mail]", 29);
        this._addSpacer();
        this._addButton("Image", "[img]", "[/img]", 21);
        this._addButton("Youtube", "[youtube]", "[/youtube]", 30);
    };
    /**
     * Insert a tag into the textbox
     * 
     * @param String start
     * @param String end
     */
    this.insert = function(start, end)
    {
        if (!end)
        {
            end = "";
        }

        var range = {start:this.element.get(0).selectionStart, end: this.element.get(0).selectionEnd};
        var text = this.element.val();
        var caret = 0;
        
        if(range.start != range.end)
        {
            var first = text.substring(0, range.start) + start + text.substring(range.start, range.end);
            
            this.element.val(first + end + text.substring(range.end))
            caret = first.length;
        }
        else
        {
            this.element.val(text.substring(0, range.start) + start + end + text.substring(range.end))
            caret = range.end + (start + end).length;
        }
        
        this.element.focus();
        this.element.get(0).setSelectionRange(caret, caret);
    };
    /**
     * Add a button to the toolbar
     * 
     * @param String text
     * @param String start
     * @param String end
     * @param int pixID
     */
    this._addButton = function(text, start, end, pixID)
    {
        var button = $("<a href=\"#\" class=\""+text+"\"></a>");
        button.bind("click", {start: start, end:end, editor:this}, this.handelClick);
        button.css("background-position", "0 "+((pixID - 1) * -30)+"px");
        button.bind("mouseover", {editor:this}, this._handelMouseOver);
        button.bind("mouseout", {editor:this}, this._handelMouseOut);
        this._addToolbarElement(button);
    };
    /**
     * Add a spacer to the toolbar
     */
    this._addSpacer = function()
    {
        var button = $("<span class='spacer'/>");
        this._addToolbarElement(button);
    };
    /**
     * Handel clicks on a button on the toolbar
     * 
     * @param Event e
     * @returns Boolean
     */
    this.handelClick = function(e)
    {
        e.data.editor.insert(e.data.start, e.data.end);
        
        return false;
    };
    /**
     * Handel mouse over to display hover animarion
     * 
     * @param Event e
     */
    this._handelMouseOver = function(e)
    {
        var button = $(e.currentTarget);
        var pos = e.data.editor._parsePos(button.css("background-position"));
        button.css("background-position", "34px "+pos[1]+"px");
    };
    /**
     * Handel mouse out to hide hover animarion
     * 
     * @param Event e
     */
    this._handelMouseOut = function(e)
    {
        var button = $(e.currentTarget);
        var pos = e.data.editor._parsePos(button.css("background-position"));
        button.css("background-position", "0 "+pos[1]+"px");
    };
    /**
     * Parse a string so we get a array with cooridates
     * 
     * @param String pos
     * @returns Array       [int x, int y]
     */
    this._parsePos = function(pos)
    {
        var posArr = pos.split(" ");
        posArr[0].replace("px", "");
        posArr[1].replace("px", "");
        posArr[0] = parseInt(posArr[0]);
        posArr[1] = parseInt(posArr[1]);
        
        return posArr;
    };
    /**
     * Wrap the toolbar around the element
     * 
     * @param DOMElement html
     */
    this._addToolbarElement = function(html)
    {
        //wrap the toolbar in a div if it didn't happen already
        this.toolbar.append(html);
    };
    this.init(element, options); //call constructor
}

/**
 * TODO refactor in Object the functions:
 *   deleteMultiple
 *   getComponent
 *   postComponent
 *   openForm
 *   openTable
 *   openFormPopup
 *   closeFormPopup
 */

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
    getComponent("core", "getform", {component: component, form: action}, openFormPopup);
    return false;
}
function openTable(table, action)
{
    getComponent("core", "getform", {table: table, form: action}, openFormPopup);
    return false;
}
function openFormPopup(data)
{
    $("#formWrapper").html(decodeURIComponent(data.html));
    $("#formWrapperBackground").css("display", "block");
}
function closeFormPopup()
{
    $("#formWrapperBackground").css("display", "none");
    return false;
}

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
    };
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
/**
 * TODO refactor in Object the functions:
 *   file_upload
 *   file_getType
 *   file_dragenter
 *   file_dragout
 *   file_bindListeners
 */
function file_upload(event) {
    var data = event.dataTransfer;
    
    if(data && data.files.length > 0)
    {
        var boundary = '------multipartformboundary' + (new Date).getTime();
        var dashdash = '--';
        var crlf     = '\r\n';
    
        /* For each dropped file. */
        for (var i = 0; i < data.files.length; i++) 
        {
            /* Build RFC2388 string. */
            var builder = '';
            var file = data.files[i];
            
            //form data
            builder += dashdash+boundary+crlf;
            builder += 'Content-Disposition: form-data; name="component"';
            builder += crlf+crlf;
            builder += "file"
            builder += crlf;
            
            builder += dashdash+boundary+crlf;
            builder += 'Content-Disposition: form-data; name="action"';
            builder += crlf+crlf;
            builder += "upload"
            builder += crlf;
            
            
            /* Generate headers. */
            builder += dashdash;
            builder += boundary;
            builder += crlf;
            builder += 'Content-Disposition: form-data; name="file_data"';
            if (file.fileName)
            {
              builder += '; filename="' + file.fileName + '"';
            }
            builder += crlf;
    
            builder += 'Content-Type: ';
            if(file.type)
            {
                builder += file.type
            }
            else
            {
                builder += 'application/octet-stream'
            }
            builder += crlf;
            builder += crlf; 
    
            /* Append binary data. */
            builder += file.getAsBinary();
            builder += crlf;
            
            /* Mark end of the request. */
            builder += dashdash;
            builder += boundary;
            builder += dashdash;
            builder += crlf;
        
            var xhr = new XMLHttpRequest();
            xhr.open("POST", "/", true);
            xhr.setRequestHeader('Content-Type', 'multipart/form-data; boundary=' + boundary);
            xhr.setRequestHeader('X-File-Name', file.fileName);
            xhr.setRequestHeader('X-File-Size', file.fileSize);
            
            //add Loader to the tree
            var parent = uploadFolder.nodes[file_getType(file.type)];
            var nodes = new TreeItem(file.name.substr(0, file.name.lastIndexOf(".")), parent, null, uploadFolder.root, "loader");
            uploadFolder.root.addChild(nodes);
            nodes.parent.open();
            
            //add the node to the loader
            xhr.node = nodes;
            
            xhr.onload = function(event) { 
                /* If we got an error display it. */
                if (event.currentTarget.responseText)
                {
                    var data = $.parseJSON(event.currentTarget.responseText);
                    if(data.error)
                    {
                        event.currentTarget.node.parent.removeChild(event.currentTarget.node);
                    }
                    else
                    {
                        event.currentTarget.node.icon.attr("src", event.currentTarget.node.root.getIcon(file_getType(data.type)));
                        event.currentTarget.node.link.attr("href", data.editLink)
                        event.currentTarget.node.addAction(new TreeAction("view", data.viewLink, event.currentTarget.node.root));
                        event.currentTarget.node.addAction(new TreeAction("delete", data.deleteLink, event.currentTarget.node.root));
                    }
                }
            };
            
            xhr.sendAsBinary(builder); //TODO This needs to be fixed when there is an update to the firefox File API
        }
    }
    
    file_dragout(event);
    
    event.stopPropagation();
    event.preventDefault();
}
function file_getType(fileType)
{
    if(fileType.match(/^image\/.*$/i))
    {
        return "images";
    }
    else if(fileType.match(/^audio\/.*$/i))
    {
        return "vid";
    }
    else if(fileType.match(/^video\/.*$/i))
    {
        return "vid";
    }
    else if(fileType.match(/^application\/vnd\.openxmlformats\-officedocument\.spreadsheetml\.sheet$/i))
    {
        return "docs";
    }
    else if(fileType.match(/^application\/vnd\.openxmlformats\-officedocument\.wordprocessingml\.document$/i))
    {
        return "docs";
    }
    else if(fileType.match(/^application\/vnd\.ms-powerpoint$/i))
    {
        return "docs";
    }
    else if(fileType.match(/^application\/msword$/i))
    {
        return "docs";
    }
    else if(fileType.match(/^application\/vnd\.oasis\..*$/i))
    {
        return "docs";
    }
    else 
    {
        return "other";
    }
}
function file_dragenter(event)
{
    $(event.currentTarget).addClass("fileFocus");
    
    event.stopPropagation();
    event.preventDefault();
}
function file_dragout(event)
{
    $(event.currentTarget).removeClass("fileFocus");
    
    event.stopPropagation();
    event.preventDefault();
}
function file_bindListeners(element)
{
    if(XMLHttpRequest.prototype.sendAsBinary) //check if we can actully upload (Firefox 3.6+ only)
    {
        element.addEventListener('drop', file_upload, false);
        element.addEventListener('dragenter', file_dragenter, false);
        element.addEventListener('dragover', file_dragenter, false);
        element.addEventListener('dragout', file_dragout, false);
        element.addEventListener('dragleave', file_dragout, false);
    }
}

/**
 * TODO refactor in Object the functions:
 *   adminmenu_openMenu
 *   adminmenu_hideItem
 *   adminmenu_closeMenu
 */
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

/**
 * Tag Field class for handeling the autocompletion
 */
var TagField = function(element, tags)
{
    this.element;
    this.tags;
    this.panel;
    
    /**
     * Constructor
     * 
     * @param JQueryObject element
     * @param Array tags
     */
    this.init = function(element, tags)
    {
        this.element = element;
        this.element.attr("autocomplete", "off");
        var pos = element.position();
        this.tags = tags.sort();
        this.panel = $("<ul class='tagPanel'></ul>");
        this.panel.css({left: pos.left, top: pos.top + this.element.outerHeight(), display: "none", width: this.element.innerWidth()});
        
        $("body").append(this.panel);
        
        this.element.focus(this.handelFocus);
        this.element.blur(this.handelBlur);
        this.element.keyup(this.handelKey);
        this.element.keydown(this.handelKeyDown);
        this.element.get(0).field = this;
    };
    /**
     * Handel when the field get focus
     * 
     * @param Event e
     */
    this.handelFocus = function(e)
    {
        $(this).trigger("keyup");
    };
    /**
     * Handel when the field looses focus
     * 
     * @param Event e
     */
    this.handelBlur = function(e)
    {
        this.field.panel.css("display", "none");
        this.field.panel.empty();
    };
    /**
     * Handel when enter or down is pressed to add the first selected tag
     * 
     * @param Event e
     * @return Boolean
     */
    this.handelKeyDown = function(e)
    {
        if(e.keyCode == 40 || e.keyCode == 13)
        {
            if(this.field.panel.children("li").length > 0)
            {
                this.field.selectTag(this.field.panel.children("li:first").text());
            }
            return false;
        }
    };
    /**
     * Select a tag to add to the field
     * 
     * @param String tag
     */
    this.selectTag = function(tag)
    {
        var curVal = this.element.val();
        
        if(curVal.lastIndexOf(",") >= 0)
        {
            this.element.val(curVal.substring(0, curVal.lastIndexOf(",")) + ", " + tag + ", ");
        }
        else
        {
            this.element.val(tag + ", ");
        }
    };
    /**
     * Handel when a key is pressed to update the tag box
     * 
     * @param Event e
     * @return Boolean
     */
    this.handelKey = function(e)
    {
        var tags = $(this).val().split(",");
        
        for(var i = 0; i < tags.length; i++)
        {
            tags[i] = tags[i].replace(/^\s\s*/, '').replace(/\s\s*$/, '');
        }
        
        var input = tags.pop();
        
        if(input != "")
        {
            var count = 0;
            this.field.panel.empty();
            
            for(var i = 0; i < this.field.tags.length; i++)
            {
                var tag = this.field.tags[i];
                
                if(tag.indexOf(input) == 0 && tags.indexOf(tag) == -1 && tag != input)
                {
                    this.field.panel.append("<li " + ((count == 0)? "class='first'" : "") + ">"+tag+"</li>");
                    count++;
                }
            }
        }
        
        if(count == 0 || input == "")
        {
            this.field.panel.css("display", "none");
            this.field.panel.empty();
        }
        else
        {
            this.field.panel.css("display", "block");
        }
        
        return false;
    };
    this.init(element, tags); //call constructor
}