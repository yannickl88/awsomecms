{foreach from=$news item=item}
    <div>
        <a href="/news?id={$item->news_id}"><h1>{$item->news_title}</h1></a>
        <div class="newsPoster">Posted by <a href="/user?id={$item->user_id}">{$item->user_name}</a> on {$item->news_date}</div>
        <div>{$item->news_text}</div>
    </div>
{/foreach}