<div>
    <h1>{$news->news_title}</h1>
    <div class="newsPoster">Posted by <a href="/user?id={$news->user_id}">{$news->user_name}</a> on {$news->news_date}</div>
    <div>{$news->news_text}</div>
</div>