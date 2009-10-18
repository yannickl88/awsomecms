{if $FORMERROR}
    <div class="form_error">
        The form was not filled in correctly. Please check the following fields:
        <ul id="errorlist">
        </ul>
    </div>
    <script>
        {foreach from=$FORMERROR.fields key=field item=error}
            addFieldError('{$field}', '{$error}');
        {/foreach}
    </script>
{/if}
<form method="post" action="/" class="admin_form admin_news_add">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="news_title">Title:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="news_title" id="news_title" value="{$FORMERROR.record.news_title}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="news_user">User:</label>
        </div>
        <div class="admin_form_field">
            <select name="news_user" id="news_user">
                {foreach from=$users item=user}
                    <option value="{$user->user_id}" {if $FORMERROR.record.news_user == $user->user_id}selected="selected"{/if}>{$user->user_name}</option>
                {/foreach}
            </select>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="news_date_Month">Date:</label>
        </div>
        <div class="admin_form_field">
            {html_select_date prefix="news_date_" time=$FORMERROR.record.news_date day_extra="id=news_date_Day" month_extra="id=news_date_Month" year_extra="id=news_date_Year"}
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="news_tag">Tag:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="news_tag" id="news_tag" value="{$FORMERROR.record.news_tag}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="news_text">Content:</label>
        </div>
        <div class="admin_form_field">
            <textarea name="news_text" id="news_text" rows="5" cols="20"></textarea>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="news_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add" />
    <input type="hidden" name="component" value="news" />
</form>