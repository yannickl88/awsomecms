{foreach from=$news item=item}
    <h2><a href="/news?id={$item->news_id}">{$item->news_title}</a></h2>
    {$item->news_text|truncate:50}
    <div style="float: right;">
        <a href="/news?id={$item->news_id}">Lees meer >>></a>
    </div>
    <hr />
{/foreach}