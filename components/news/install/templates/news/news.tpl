{foreach from=$news item=item}
    <div>
        {if $item->news_external == 1}
            <a href="{$item->news_extlink}" target"_blank">
        {else}
            <a href="/news?id={$item->news_id}">
        {/if}
        <h1>{$item->news_title}</h1></a>
        <div class="newsPoster">Posted by <a href="/user?id={$item->user_id}">{$item->user_name}</a> on {$item->news_date}</div>
        <div>{$item->news_text}</div>
    </div>
{/foreach}