GALLERY
{foreach from=$gallery->images item=item}
    {include file="gallery/gallery_item.tpl" item=$item}
{/foreach}