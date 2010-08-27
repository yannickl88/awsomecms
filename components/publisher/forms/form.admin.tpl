<h2>{"publishoverview"|text}</h2>
{foreach from=$items item=item name=elements}
<div id="{$item->id}" class="publishItem{if $smarty.foreach.elements.index % 2 == 1} highlighted{/if}">
    <div class="publishTitle">
        <span class="options">
            {"publishon"|text}
            {foreach from=$publishers item=$publisher name=publishers key=pubName}
                {if $item->published->$pubName}
                <img src="/img/icons/publish-{$pubName}-done.png" alt="{$pubName}" title="{$pubName}"/>
                {elseif $item->publishers->$pubName}
                <a href="#" class="publish"><img src="/img/icons/publish-{$pubName}.png" alt="{$pubName}" title="{$pubName}"/></a>
                {else}
                <img src="/img/icons/publish-{$pubName}-no.png" alt="{$pubName}" title="{$pubName}"/>
                {/if}
            {/foreach}
        </span>
        <span class="title">
            <span class="type">{$item->type|text|ucfirst}:</span>{$item->title}
        </span>
    </div>
    <div class="publishBody{if $smarty.foreach.elements.first} selected{/if}">
        <div class="image">
            {if $item->picture != ""}
            <img src="{$item->picture}" width="100"/>
            {/if}
        </div>
        <div class="content">
            <div class="message">
                {$item->message|bbcode}
            </div>
        </div>
        <div class="clear"></div>
    </div>
</div>
{/foreach}

<script type="text/javascript">
//<![CDATA[
    function publish_mouseOver(e)
    {
        //hide current selected
        $(".publishItem .publishBody.selected").removeClass("selected");
        $(this).find(".publishBody").addClass("selected");
    }
    function publish_publish(e)
    {
        var to = $(this).find("img").attr("alt");
        var id = encodeURI($(this).parents(".publishItem").attr("id"));
        var icon = $("<img src='/img/admin/loader.gif' alt='loading' class='loader "+publish_fixID(id)+"'/>");
        icon.get(0).to = to;
        
        $(this).replaceWith(icon);
        var win = window.open("/?component=publisher&action=auth&id="+id+"&to="+to, "publish", "menubar=no,width=640,height=480,toobar=no");
        var timerID = window.setInterval(function() {
            publish_checkPopup(win, id, timerID);
        }, 1000);
        
        win.loader = icon;
        win.timer = timerID;
        
        return false;
    }
    function publish_done(popup, id)
    {
        var node = $(".loader."+publish_fixID(id));
        var to = node.get(0).to;
        var icon = $("<img src='/img/icons/publish-"+to+"-done.png' alt='"+to+"' title='"+to+"'/>");
        node.replaceWith(icon);

        if(popup) {
            window.clearInterval(popup.timer); 
            popup.close();
        } 
    }
    function publish_checkPopup(popup, id, timerID)
    {
        if(!popup.opener) {
            window.clearInterval(timerID);
             
            var node = $(".loader."+publish_fixID(id));
            var to = node.get(0).to;
            var icon = $("<img src='/img/icons/publish-"+to+".png' alt='"+to+"' title='"+to+"'/>");
            var link = $("<a href='#' class='publish'></a>");
            link.append(icon);
            link.click(publish_publish);
            
            node.replaceWith(link);
        }
    }
    function publish_fixID(id)
    {
        return id.replace(/::/g, '_').replace(/\./g, '_');
    }
    $().ready(function(e) {
        $(".publishItem").mouseover(publish_mouseOver);
        $(".publishItem .options .publish").click(publish_publish);
    });
//]]>
</script>