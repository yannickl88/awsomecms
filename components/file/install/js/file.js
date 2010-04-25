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
            
            console.log(file);
            
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
            xhr.setRequestHeader('content-type', 'multipart/form-data; boundary=' 
                + boundary);
            xhr.sendAsBinary(builder);
            
            //add Loader to the tree
            var parent = uploadFolder.nodes[file_getType(file.type)];
            var node = new TreeItem(file.name.substr(0, file.name.lastIndexOf(".")), parent, null, uploadFolder.root, "loader");
            uploadFolder.root.addChild(node);
            node.parent.open();
            
            xhr.onload = function(event) { 
                
            };
        }
    }
    
    event.stopPropagation();
}
function file_getType(fileType)
{
    if(fileType.match(/^image\/*$/i))
    {
        return "image";
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
function file_dragover(event)
{
    $(event.currentTarget).addClass("fileFocus");
    event.stopPropagation();
    event.preventDefault();
}
function file_dragout(event)
{
    $(event.currentTarget).removeClass("fileFocus");
}