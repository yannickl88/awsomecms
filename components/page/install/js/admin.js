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
    this.resizebar = null;
    
    this.resizing = false;
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
            this.resizebar = $("<div class='toolbarresize'><div class='resize'></div></div>");
            this.element.parent().prepend(this.toolbar);
            this.element.parent().append(this.resizebar);
            
            //bind the resize
            this.resizebar.bind("selectstart", function(e) { return false;});
            this.resizebar.bind("mousedown", {editor: this}, function(e) { e.data.editor.resize(e); return false;});
        }
        
        if(options != undefined)
        {
            if(options.width != undefined) 
                this.element.parent().width(options.width);
                this.element.css({width: options.width - 5, resize: "none"});
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
        this._addButton("Link", "[link code=]", "[/link]", 31);
        this._addButton("E-mail", "[mail]", "[/mail]", 29);
        this._addSpacer();
        this._addButton("Image", "[img]", "[/img]", 21);
        this._addButton("Youtube", "[youtube]", "[/youtube]", 30);
    };
    this.resize = function(e) {
        var editor = this; //local var for refering in listeners
        var originalY = e.pageY;
        var prevHeight = this.element.height();
        var moveHandler = function(e) {
            editor.element.height(prevHeight + (e.pageY - originalY));
        };
        
        $("body").mousemove(moveHandler);
        $("body").mouseup(function(e) {
            $("body").unbind("mousemove", moveHandler);
            $(this).unbind(e);
        });
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
            
            this.element.val(first + end + text.substring(range.end));
            caret = first.length;
        }
        else
        {
            this.element.val(text.substring(0, range.start) + start + end + text.substring(range.end));
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
};

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

function openForm(component, action, callback)
{
    getComponent("core", "getform", {component: component, form: action}, function(data) {
        openFormPopup(data);
        if(callback != null) callback();
    });
    return false; 
}
function openTable(table, action, callback)
{
    getComponent("core", "getform", {table: table, form: action}, function(data) {
        openFormPopup(data);
        if(callback != null) callback();
    });
    return false; 
}
function openFormPopup(data)
{
    var html = $(Base64.decode(data.html));
    $("#formWrapper").html(html);
    $("#formWrapperBackground").css("display", "block");
    $("#formHeaderWrapper").css("display", "block");
    
    return false;
}
function closeFormPopup()
{
    $("#formWrapperBackground").css("display", "none");
    
    return false;
}

/**
 * Admin tree with nodes
 */
var Tree = function(element, checkboxes, systemFileTree)
{
    this.children = new Array();
    this.contentType = 1;
    this.iconMap = {};
    this.html = null;
    this.javascript = false;
    this.foldersOnly = false;
    this.selectedFolder = "/";
    this.checkboxes = checkboxes[0];
    this.checkboxValues = (checkboxes[1] != "") ? checkboxes[1] : [];
    this.noCookie = false;
    this.systemFileTree = (systemFileTree === true);

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
                if((!this.foldersOnly || child.isLoader) || child.type != Tree.PAGE)
                {
                    this.html.append(child.html);
                }
                this.children.push(child);
            }
            else
            {
                if((!this.foldersOnly || child.isLoader) || child.type != Tree.PAGE)
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
                    this.children[x].html.before(folder.html);
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
    this.addCheckboxValues = function(id) {
        if(this.checkboxes == 2) {
            this.checkboxValues[0] = id;
        } else if(this.checkboxes == 3) {
            for(var i in this.checkboxValues) {
                if(this.checkboxValues[i] == id) {
                    return;
                }
            }
            this.checkboxValues.push(id);
        }
        this.updateCheckboxValues();
    };
    this.removeCheckboxValues = function(id) {
        if(this.checkboxes == 2) {
            this.checkboxValues = [];
        } else if(this.checkboxes == 3) {
            for(var i in this.checkboxValues) {
                if(this.checkboxValues[i] == id) {
                    this.checkboxValues.splice(i, 1);
                    this.updateCheckboxValues();
                    return;
                }
            }
        }
    };
    this.updateCheckboxValues = function() {
        this.html.find("input[type=hidden].treeValues").remove();
        if(this.checkboxes == 2) {
            this.html.append("<input type='hidden' name='"+this.javascript+"' value='"+this.checkboxValues[0] + "' class='treeValues'/>");
        } else if(this.checkboxes == 3) {
            for(var i in this.checkboxValues) {
                this.html.append("<input type='hidden' name='"+this.javascript+"[]' value='"+this.checkboxValues[i] + "' class='treeValues'/>");
            }
        }
    };

    this.init(element);
};
// Constants
Tree.PAGE = 1;
Tree.PAGE_FOLDER = 2;

/**
 * Folder page item
 */
var TreeItem = function(name, location, url, root, icon, actions, contentType, id)
{
    this.name = name;
    this.location = location;
    this.parent = null;
    if(url && url.substr(0, 1) != "/") this.url = "/"+url; else this.url = url;
    this.type = Tree.PAGE;
    this.contentType = contentType;
    this.html = null;
    this.root = root;
    this.actions = actions;
    this.icon = $("<img alt='page' src='"+this.root.getIcon(icon)+"' />");
    this.link = null;
    this.id = id;
    this.isLoader = (icon == "loader");
    
    /**
     * Constructor
     */
    this.init = function()
    {
        var ref = this;
        this.html = $("<div class='hideIcons'></div>");
        this.html[0].object = this;
        if(this.url && this.url != "") {
            this.link = $("<a href='"+this.url+"'>"+this.name+"</a>");
            this.link.click(function(e) {
                if(ref.root.javascript)
                {
                    if(ref.root.checkboxes == 1)
                        $("#"+ref.root.javascript).val(ref.parent.getLocPath() + ref.name);
                    else {
                        var checkbox = ref.link.find("input");
                        
                        if((checkbox.attr("type") == "checkbox" && !checkbox.attr("checked")) || checkbox.attr("type") == "radio") {
                            checkbox.attr("checked", "checked");
                            
                            ref.root.addCheckboxValues(ref.id);
                        }
                        else if(checkbox.attr("type") == "checkbox" && checkbox.attr("checked")) {
                            checkbox.removeAttr("checked");
                            
                            ref.root.removeCheckboxValues(ref.id);
                        }
                    }
                    
                    return false;
                }
            });
        } else {
            this.link = $("<span class='treeItem'>"+this.name+"</span>");
        }
        this.link.prepend(this.icon);
        
        if(!this.isLoader) {
            if(this.root.checkboxes == 2) {
                var icon = $('<input type="radio" value="'+ this.id +'" name="'+this.root.javascript+'_dummy"/>');
                if(this.root.checkboxValues == this.id)
                    icon.attr("checked", true);
                this.link.prepend(icon);
            } else if(this.root.checkboxes == 3) {
                var icon = $('<input type="checkbox" value="'+ this.id +'" name="'+this.root.javascript+'_dummy[]"/>');
                for(var i in this.root.checkboxValues) {
                    if(this.root.checkboxValues[i] == this.id) {
                        icon.attr("checked", true);
                        break;
                    }
                }
                this.link.prepend(icon);
            }
        }
        this.html.append(this.link);
        if(this.actions)
        {
            for(var x in this.actions)
            {
                this.actions[x].html.css("margin-left", -20 * x);
                this.addAction(this.actions[x]);
            }
        }
        if(!ref.root.javascript)
        {
            this.html.mouseover(function(e){
                $(this).removeClass("hideIcons");
            });
            this.html.mouseout(function(e){
                $(this).addClass("hideIcons");
            });
        }
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
};
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
    this.loaded = false;
    this.queue = [];
    
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
                this.actions[x].html.css("margin-left", -20 * x);
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
        if(!this.root.foldersOnly && !ref.root.javascript)
        {
            this.html.mouseover(function(e) {
                var div = $(e.target).parent(".treeNode div").get(0);
                if(div && div.object) {
                    var node = $(e.target).parent(".treeNode div")[0].object;
                    if(node.type == Tree.PAGE_FOLDER)
                        node.root.selectedFolder = node.location + node.name + "/";
                }
                $(this).removeClass("hideIcons");
            });
            this.html.mouseout(function(e){
                $(this).addClass("hideIcons");
            });
        }
        
        //open when we have a cookie
        if(this.hasCookie(this.getPath()) && !this.root.noCookie)
        {
            this.loaded = true;
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
                if(this.children[x].type < child.type)
                {
                    break;
                }
                x++;
            }
            if(x == this.children.length)
            {
                if((!this.root.foldersOnly || child.isLoader) || child.type != Tree.PAGE)
                {
                    this.html.children(".treeNode").append(child.html);
                }
                this.children.push(child);
            }
            else
            {
                if((!this.root.foldersOnly || child.isLoader) || child.type != Tree.PAGE)
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
                if(this.children[x].type < folder.type)
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
                this.children[x].html.before(folder.html);
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
    this.open = function(noload) 
    {
        this.html.children(".treeNode").removeClass("hidden");
        this.html.children(".clickableTreeArrow").addClass("open");
        this.openFolder = true;
        
        if(!this.root.noCookie)
            this.createCookie(this.getPath(), 1);
        
        //load data
        if(!this.loaded && noload !== true) {
            var loaderNode = new TreeItem("loading...", this.getPath()+"/", null, this.root, "loader", [], 2, -1);
            this.addChild(loaderNode, []);
            var root = this.root;
            var node = this;
            $.getJSON("/", {
                "action": "loadTree",
                "component": "page",
                "path": this.getPath()
            }, function(e) {
                node.removeChild(loaderNode);
                
                for(var i=0; i < e.length ; i++) {
                    var data = e[i];
                    
                    if(data.classname == "TreeFolder")
                        root.addChild(new TreeFolder(data.name, data.location, root, data.icon, createActionsFromData(data.actions, root), data.contentType, data.isRootFolder));
                    else if(data.classname == "TreeNode")
                        root.addChild(new TreeItem(data.name, data.location, data.url, root, data.icon, createActionsFromData(data.actions, root), data.contentType, data.id));
                    else if(data.classname == "TreeUploadFolder")
                    {
                        var child = new TreeFolder(data.name, data.location, root, data.icon, createActionsFromData(data.actions, root), data.contentType, data.isRootFolder);
                        if(root.systemFileTree) {
                            uploadFolder = child
                            uploadFolder.nodes = data.nodes;
                            $().ready(function(e) {
                                new FileUpload(uploadFolder.html.get(0));
                            });
                        }
                        root.addChild(child);
                    }
                }
                
                for(var j = 0; i < node.queue.length ; i++) {
                    root.addChild(node.queue[i]);
                }
                node.queue = [];
            });
            this.loaded = true;
        }
    };
    /**
     * Collapse the folder
     */
    this.close = function()
    {
        if(this.openFolder) {
            this.html.children(".treeNode").addClass("hidden");
            this.html.children(".clickableTreeArrow").removeClass("open");
            this.openFolder = false;
            
            if(!this.root.noCookie)
                this.eraseCookie(this.getPath());
            
            for(var i=0; i < this.children.length; i++) {
                if(this.children[i].close)
                    this.children[i].close();
            }
        }
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
};

function createActionsFromData(actionData, root) {
    var actions = [];
    
    for(var i = 0; i < actionData.length; i++) {
        actions.push(new TreeAction(actionData[i].icon, actionData[i].link, root))
    }
    
    return actions;
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
    if(url.substr(0, 1) != "/" && url.substr(0, 7) != "http://")
        this.url = "/"+url;
    else
        this.url = url;
    this.html = null;
    this.root = root;
    
    this.init = function()
    {
        this.html = $("<a class='treeIcon "+this.icon+"' href='"+this.url+"'><img src='"+this.root.getIcon(this.icon)+"' alt='tree action'/></a>");
    };
    
    this.init();
};
var FileUpload = function(element) {
    this.element = element;
    
    this.init = function() {
        this.bind();
    };
    this.getType = function(fileType) {
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
    };
    this.dragenter = function(e) {
        $(e.currentTarget).addClass("fileFocus");
        
        e.stopPropagation();
        e.preventDefault();
    };
    this.dragout = function(e) {
        $(e.currentTarget).removeClass("fileFocus");
        
        e.stopPropagation();
        e.preventDefault();
    };
    this.bind = function() {
        var upload = this;
        
        //bind the listeners if we comply to the HTML5 spec
        if(typeof FormData == 'function') {
            this.element.addEventListener('drop', function(e) {
                upload.upload(e);
            }, false);
            this.element.addEventListener('dragenter', function(e) {
                upload.dragenter(e);
            }, false);
            this.element.addEventListener('dragover', function(e) {
                upload.dragenter(e);
            }, false);
            this.element.addEventListener('dragout', function(e) {
                upload.dragout(e);
            }, false);
            this.element.addEventListener('dragleave', function(e) {
                upload.dragout(e);
            }, false);
            $(this.element).find("a.reload").click(function(e) {
                var img = $(this).find("img");
                img.attr("src", upload.element.object.root.getIcon("loader"));
                $.getJSON("/", {"component": "file", "action": "update"}, function(data) {
                    for(var i in data) {
                        var childData = data[i];
                        
                        var actions = [];
                        for(var i in childData.actions) {
                            actions.push(new TreeAction(childData.actions[i].icon, childData.actions[i].link, upload.element.object.root));
                        }
                        var node = new TreeItem(childData.name, childData.location, childData.editLink, upload.element.object.root, childData.icon, actions, childData.type);
                        upload.element.object.root.addChild(node);
                    }
                    
                    img.attr("src", upload.element.object.root.getIcon("reload"));
                });
                return false;
            });
            
            this.element.object.upload = this;
        }
    };
    this.fixLocation = function(treeNode, type, location) {
        var properLoc = "/"
        if(location.indexOf(treeNode.nodes[type]) == 0) { // part of the subset
            properLoc = location.substr(treeNode.nodes[type].length - 1);
        }
        
        return properLoc;
    };
    this.upload = function(e) {
        var data = e.dataTransfer;
        
        if(data && data.files.length > 0)
        {
            var loaded = 0;
            var needsToLoad = data.files.length;
            /* For each dropped file. */
            for ( var i = 0; i < data.files.length; i++) {
                var file = data.files[i];
                
                var treeNode = this.element.object;
                var location = this.fixLocation(treeNode, this.getType(file.type), this.element.object.root.selectedFolder)
                var formData = new FormData();
                formData.append("component", "file");
                formData.append("action", "upload");
                formData.append("file_location", location);
                formData.append("file_data", file);
                
                //add Loader to the tree
                var parent = treeNode.nodes[this.getType(file.type)] + location.substr(1);
                var loaderNode = new TreeItem(file.name.substr(0, file.name.lastIndexOf(".")), parent, null, treeNode.root, "loader");
                treeNode.root.addChild(loaderNode);
                loaderNode.parent.open(true);
                
                // create a new xhr object
                var xhr = new XMLHttpRequest();
                xhr.node = loaderNode;
                // add listeners
                var u = this;
                xhr.onload = function(event) {
                    /* If we got an error display it. */
                    if (event.currentTarget.responseText)
                    {
                        var node = event.currentTarget.node;
                        var data = $.parseJSON(event.currentTarget.responseText);
                        
                        if(data.error)
                        {
                            node.parent.removeChild(node);
                        }
                        else
                        {
                            if(node.parent.loaded == true) {
                                node.icon.attr("src", node.root.getIcon(u.getType(data.type)));
                                
                                var link =  $("<a href='/"+data.editLink+"'></a>");
                                link.append(node.link.html())
                                node.link.replaceWith(link);
                                node.addAction(new TreeAction("view", data.viewLink, node.root));
                                node.addAction(new TreeAction("delete", data.deleteLink, node.root));
                                
                                // check if there any select fields
                                $(".admin_FileSelectField").each(function(key, element) {
                                    if(FileUpload.filters[$(element).attr("id")]) {
                                        var type = data.type.split("/")[1];
                                        
                                        for (var fType in FileUpload.filters[$(element).attr("id")]) {
                                            if(type == fType) {
                                                $(element).append("<option value='" + data.id + "'>" + data.location + data.name + "</option>");
                                                break;
                                            }
                                        }
                                    }
                                });
                            } else {
                                loaded++;
                                node.parent.removeChild(node);
                                
                                if(loaded == needsToLoad) {
                                    node.parent.open(false);
                                }
                            }
                        }
                    }
                };

                xhr.open("POST", "/");
                xhr.send(formData);
            }
        }
        
        this.dragout(e);
        
        e.stopPropagation();
        e.preventDefault();
    };
    
    this.init(); //call constructor
};
FileUpload.filters = {};
FileUpload.addTypeFilter = function(id, filters) {
	FileUpload.filters[id] = filters;
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
        if(element.length > 0) {
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
        }
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
};

var cField = function(element) {
    this.element = element;
    this.value = $("<input type='hidden' name='"+element.attr("id")+"' value='0;0;0;0' />");
    this.nodes = {
        1 : {},
        2 : {},
        3 : {},
        4 : {}
    };
    this.selected = [0,0,0,0];
    this.types = {
        "machine" : 1,
        "model" : 2,
        "type" : 3,
        "brand" : 4
    };
    this.translations = {
        "machine" : "Machine",
        "model" : "Model",
        "type" : "Type",
        "brand" : "Brand"
    };
    
    this.init = function() {
        this.value.data('setValue', this);
        
        for(var k in this.types) {
            var row = $("<div class='admin_categoryRow type_"+this.types[k]+"'></div>");
            var link = $("<a class='admin_categoryAdd' href='#' style='display:none;'><img src='/img/icons/add.png' alt='add'/></a>");
            
            link.bind("click", {"type": this.types[k], "field": this}, function(e) {
                var selectedE = $(e.data.field.findSelected(e.data.type - 1));
                e.data.field.showAddFialoge(e.data.type);
                
                e.stopPropagation();
                e.preventDefault();
                return false;
            });
            
            row.append("<div class='admin_categorylabel'>" + this.translations[k] + ":</div>");
            
            var select = $("<select disabled='disabled' class='admin_categorySelect'></select>");
            select.bind("change", {"type": this.types[k], "field": this}, function(e) {
                e.data.field.update(e.data.type);
            });
            
            row.append(select);
            row.append(link);
            this.element.append(row);
        }
        var row = $("<div class='admin_categoryRow'></div>");
        this.element.append(this.value);
        this.element.append(row);
    };
    
    this.findSelected = function(type) {
        var value = this.element.find("div.admin_categoryRow.type_"+type+" select.admin_categorySelect").val();
        return this.element.find("div.admin_categoryRow.type_"+type+" select.admin_categorySelect option[value="+value+"]");
    };
    
    this.setActive = function(type) {
        if(this.element.find("div.admin_categoryRow.type_"+(type)+" select.admin_categorySelect option").length > 0)
            this.element.find("div.admin_categoryRow.type_"+(type)+" select.admin_categorySelect").removeAttr("disabled");
        else
            this.element.find("div.admin_categoryRow.type_"+(type)+" select.admin_categorySelect").attr("disabled", "disabled");
        this.element.find("div.admin_categoryRow.type_"+(type)+" .admin_categoryAdd").css("display", "inline");
    };
    
    this.setInactive = function(type) {
        this.element.find("div.admin_categoryRow.type_"+(type)+" select.admin_categorySelect").attr("disabled", "disabled");
        this.element.find("div.admin_categoryRow.type_"+(type)+" .admin_categoryAdd").css("display", "none");
    };
    
    this.add = function(name, id, type, update) {
        if(typeof(update) == "undefined")
            update = true;
        
        this.nodes[type][id] = {"name": name, "id": id};
        this._addOption(type, id, name);
    };
    this.setValue = function(ids) {
        this.selected = ids.split(";");
        
        this.element.find("div.admin_categoryRow.type_1 select.admin_categorySelect").val(this.selected[0]);
        this.element.find("div.admin_categoryRow.type_2 select.admin_categorySelect").val(this.selected[1]);
        this.element.find("div.admin_categoryRow.type_3 select.admin_categorySelect").val(this.selected[2]);
        this.element.find("div.admin_categoryRow.type_4 select.admin_categorySelect").val(this.selected[3]);
    };
    this.showAddFialoge = function(type) {
        new cFieldAddDialog(type, this);
    };
    this.getParentID = function(type) {
        if(type == 1) return -1;
        
        return this.element.find("div.admin_categoryRow.type_"+(type - 1)+" select.admin_categorySelect").val();
    };
    this._addOption = function(type, id, name, selected) {
        // add select node
        if(selected)
            this.element.find("div.admin_categoryRow.type_"+type+" select.admin_categorySelect").append("<option value='"+id+"' selected='selected'>"+name+"</option>");
        else
            this.element.find("div.admin_categoryRow.type_"+type+" select.admin_categorySelect").append("<option value='"+id+"'>"+name+"</option>");
        this.element.find("div.admin_categoryRow.type_"+type+" select.admin_categorySelect").val(""+id);
        
        sort_multi_select($("div.admin_categoryRow.type_"+type+" select.admin_categorySelect"));
    };
    this.update = function(type) {
        for(var i = type + 1; i <= 4; i++) {
            this.element.find("div.admin_categoryRow.type_"+(i)+" select.admin_categorySelect").empty();
            for(var j in this.nodes[i]) {
                this._addOption(i, j, this.nodes[i][j].name, (this.selected[i - 1] == j));
            }
            
            this.element.find("div.admin_categoryRow.type_"+(i)+" select.admin_categorySelect").val(this.selected[i - 1]);
        }
        
        this.setActive(1);
        this.updateValue();
    };
    this.updateValue = function() {
        var result = "";
        
        for(var i = 4; i > 0; i--) {
            var c = this.element.find("div.admin_categoryRow.type_"+(i)+" select.admin_categorySelect option").length;
            var cp = this.element.find("div.admin_categoryRow.type_"+(i - 1)+" select.admin_categorySelect option").length;
            if(c > 0 || cp > 0)
                this.setActive(i);
            else
                this.setInactive(i);
            
            var value = this.element.find("div.admin_categoryRow.type_"+(i)+" select.admin_categorySelect").val();
            
            if(result != "") {
                result = ";" + result;
            }
            
            if(value)
                result = value + result;
            else
                result = "0" + result;
        }
        this.value.val(result);
    };
    this.init(); //call constructor
};

var cFieldAddDialog = function(type, cField) {
    this.type = type;
    this.cField = cField;
    
    this.init = function() {
        var dialoge = this;
        
        openTable("collection.mcats", "add", function() {
            $("#formWrapper .admin_form_row.admin_form_submit input[type=submit]").bind("click", {"d":dialoge}, function(e) {
                var formData = {};
                
                $("#formWrapper .admin_form_row input, .admin_form_row select").each(function(k, e) {
                    if($(e).attr("name") != "")
                        formData[$(e).attr("name")] = $(e).val();
                });
                
                e.data.d.save(formData);
                
                $(this).replaceWith("<img src='/img/admin/loader.gif' alt='loading'/>");
                $("#formHeaderWrapper").css("display", "none");
                
                e.stopPropagation();
                e.preventDefault();
                return false;
            });
        });
    };
    
    this.save = function(data) {
        data["mcat_type"] = this.type;
        
        var field = this.cField;
        
        $.post("/", data, function(data) {
            field.add(data.name, parseInt(data.id), parseInt(data.type), parseInt(data.parent));
            field.update(data.type);
            closeFormPopup();
        }, "json");
    };
    
    this.init(); //call constructor
};

function sort_multi_select(select) {
    var x = select.find('option');
    x.remove();
    x.sort(function(a,b) {
        a = a.firstChild.nodeValue.toLowerCase(); 
        b = b.firstChild.nodeValue.toLowerCase(); 
        if (a == b) 
            return 0; 
        return (a > b) ? 1 : -1; 
    });
    x.appendTo(select);
};