{foreach from=$tags item=item}
    <div class="tagItem">
        <a href="/tag?tag={$item->news_tag}" class="item">{$item->news_tag}</a>
        <a href="/tag?id={$item->news_id}" class="last">Last post on {$item->news_date}</a>
    </div>
{/foreach}