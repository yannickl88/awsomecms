{if $FORMERROR}{$FORMERROR}{/if}
<form method="post" action="/" class="form contact_send">
    <div class="form_row">
        <div class="form_label">
            <label for="comment_username">{"field_comment_username"|text}<span class="required">*</span>:</label>
        </div>
        <div class="form_field">
            <input type="text" name="comment_username" id="comment_username"/>
        </div>
    </div>
    <div class="form_row">
        <div class="form_label">
            <label for="comment_text">{"field_comment_text"|text}<span class="required">*</span>:</label>
        </div>
        <div class="form_field">
            <textarea name="comment_text" id="comment_text" rows="5" cols="30"></textarea>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="captcha">{"captcha"|text}<span class="required">*</span>:</label>
        </div>
        <div class="admin_form_field">
            <div style="height: 80px;">
                <img style="float: left;" src="/securimage_show.php?sid=d28544701be50bc66afb925278e38059" id="siimage">
                <a onclick="$('#siimage').attr('src', '/securimage_show.php?sid=' + Math.random()); return false" title="Refresh Image" href="#" style="border-style: none;" tabindex="-1">
                    <img border="0" align="bottom" onclick="this.blur()" alt="Reload Image" src="/img/refresh.gif">
                </a>
                <br />
             </div>
             Code:<br />
             <input type="text" class="empty" id="captcha" name="captcha">
         </div>
    </div>
    <div class="form_row form_submit">
        <input type="submit" value="{"send"|text|ucfirst}" id="contact_submit" class="form_submit">
	    <input type="hidden" name="comment_hook" value="{$comment_hook}" />
	    {if $comment_redirect}
	    <input type="hidden" name="comment_redirect" value="{$comment_redirect}" />
	    {/if}
	    <input type="hidden" name="action" value="reply" />
	    <input type="hidden" name="component" value="comments" />
	    <br />
	    Fields marked as <span class="required">*</span> are required. 
    </div>
</form>