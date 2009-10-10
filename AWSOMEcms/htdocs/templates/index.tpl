{$content}
{gallery}
<div id="flashbox"></div>
<script type="text/javascript">
{literal}
    $('#flashbox').flash({
        src: '/flash/PhotoGallery.swf',
        width: 490,
        height: 350,
        wmode: "transparent",
        allowScriptAccess: "always",
        flashvars: {request: "http://data.skimfo.dev"}
    });
    {/literal}
</script>