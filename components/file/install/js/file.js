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