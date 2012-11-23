{if $FORMERROR}{$FORMERROR}{/if}
<form method="post" action="/" class="form contact_send">
    {foreach from=$contactform->fields item=row}
        <div class="form_row fieldtype{$row->field_type}">
            <div class="form_label">
                <label for="{$row->field_code}">{$row->field_name}{if $row->field_required == 1}<span class="required">*</span>{/if}:</label>
            </div>
            <div class="form_field">
                {assign var=fieldcode value=$row->field_code}
                {if $row->field_type == 1}
                    <input type="text" name="{$fieldcode}" id="{$fieldcode}"/>
                {elseif $row->field_type == 2}
                    <input type="password" name="{$fieldcode}" id="{$fieldcode}"/>
                {elseif $row->field_type == 3}
                    <textarea name="{$fieldcode}" id="{$fieldcode}" rows="5" cols="30"></textarea>
                {elseif $row->field_type == 4}
                    <select name="{$fieldcode}" id="{$fieldcode}">
                        <option>{"male"|text|ucfirst}</option>
                        <option>{"female"|text|ucfirst}</option>
                    </select>
                {/if}
            </div>
        </div>
    {/foreach}
    <div class="form_row fieldtypecaptcha">
        <div class="form_field">
            <img src="/securimage_show.php?sid=d28544701be50bc66afb925278e38059" alt="captcha" class="captchaImage"/>
            <span onclick="$(this).siblings('img').attr('src', '/securimage_show.php?sid=' + Math.random()); return false" class="captchaRelead">
                <img onclick="this.blur()" alt="Reload Image" src="/img/refresh.gif" />
            </span>
         </div>
    </div>
    <div class="form_row fieldtype1">
        <div class="form_label">
            <label for="captcha">Code<span class="required">*</span>:</label>
        </div>
        <div class="form_field">
            <input type="text" class="empty" id="captcha" name="captcha" />
         </div>
    </div>
    <div class="form_row form_submit">
        <input type="submit" value="{"send"|text|ucfirst}" id="contact_submit" class="form_submit" />
	    <input type="hidden" name="form_id" value="{$contactform->form_id}" />
	    <input type="hidden" name="url" value="{$URL}" />
	    <input type="hidden" name="action" value="send" />
	    <input type="hidden" name="component" value="contact" />
    </div>
</form>