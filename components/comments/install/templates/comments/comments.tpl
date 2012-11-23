{foreach from=$comments item=comment}
    {include file="comments/comment_item.tpl" item=$comment}
{/foreach}
{include file="comments/comment_reply.tpl"}