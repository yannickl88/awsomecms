<img src="/img/nieuws.gif" alt="Nieuws"/>
<ul>
{foreach from=$news item=item}
    <li><a href="/news?id={$item->news_id}">{$item->news_title}</a></li>
{/foreach}
</ul>