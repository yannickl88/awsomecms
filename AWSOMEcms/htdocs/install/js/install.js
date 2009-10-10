var currentstep = 1;
var dependancies = {};
var dependanciesChecked = new Array();
var disabled = false;
var stepsexec = new Array();

//final data
var finalData = {
    'components[]': new Array(),
    debug: 0,
    authcomponent: 'core',
    proxy: false,
    proxy_url: '',
    proxy_user: '',
    proxy_pass: '',
    admin_needslogin: true,
    admin_location: '',
    admin_login: '',
    admin_default: false,
    dbhost: 'localhost',
    dbdatabase: '',
    dbusername: '',
    dbpassword: ''
};

function gotoStep(number)
{
    if(disabled) return;
    
    //unselected
    $('.selected').removeClass('selected');
    
    //hide all current steps
    $('#step1, #step2, #step3, #step4, #step5, #errorstep').css("display", "none");
    
    //replace old menu item
    $('#step'+currentstep+'_menu span').replaceWith('<a href="javascript: void(0)" class="filedin" onclick="gotoStep('+currentstep+');">'+$('#step'+currentstep+'_menu span').html()+'</a>');
    
    currentstep = number;
    
    $('#prev, #next').removeAttr("disabled");
    
    //toggel buttons
    if(currentstep == 5)
    {
        //change button to finish
        $('#next').text('Finish');
    }
    else
    {
        //set the next button to next
        $('#next').text('Next');
    }
    if(currentstep == 1)
    {
        $('#prev').attr("disabled", "disabled");
    }
    
    //activate current menu
    $('#step'+currentstep+'_menu span, #step'+currentstep+'_menu a').addClass('selected');
    
    switch(number)
    {
        case 2:
            $('#step2').css("display", "block");
            if(!indexOf(2, stepsexec)) {action_step2(), stepsexec.push(2)};
            break;
        case 3:
            $('#step3').css("display", "block");
            if(!indexOf(3, stepsexec)) {action_step3(), stepsexec.push(3)};
            break;
        case 4:
            $('#step4').css("display", "block");
            action_step4();
            break;
        case 5:
            $('#step5').css("display", "block");
            action_step5();
            break;
        default:
            $('#step1').css("display", "block");
            if(!indexOf(1, stepsexec)) {action_step1(), stepsexec.push(1)};
            break;
    }
}
function nextStep()
{
    if(currentstep < 5)
    {
        gotoStep(currentstep+1);
    }
    else if(currentstep == 5)
    {
        $('#prev, #next').attr("disabled", "disabled");
        disabled = true;
        
        //save
        $('#next').html("<img src='/install/img/loader.gif'/>");
        jQuery.post("/install.php?action=save", finalData, function(data) {
            $('#step1, #step2, #step3, #step4, #step5').css("display", "none");
            if(data.result)
            {
                $('#prev, #next').css("display", "none");
                $('#finalstep').css("display", "block");
            }
            else
            {
                $('#next').text('Finish');
                $('#errorstep').css("display", "block");
                $('#errorWrapper').text(data.error);
                
                $('#prev').removeAttr("disabled");
                disabled = false;
            }
        }, 'json');
    }
}
function prevStep()
{
    if(currentstep > 1)
    {
        gotoStep(currentstep-1);
    }
}
function checkDependecies(component)
{
    dependanciesChecked = new Array();
    checkDependeciesFrom(component);
    
    dependanciesChecked = new Array();
    checkDependeciesTo(component);
}
function checkDependeciesFrom(component)
{
    //check dependancies from the component
    if(!indexOf(component, dependanciesChecked) && dependancies[component])
    {
        dependanciesChecked.push(component);
        
        $(dependancies[component]).each(function(key, value) {
            $('#component_'+value).attr("checked", "checked");
            
            checkDependeciesFrom(value);
        });
    }
}
function checkDependeciesTo(component)
{
    //check dependancies from to component
    if(!indexOf(component, dependanciesChecked))
    {
        dependanciesChecked.push(component);
        
        for(var key in dependancies)
        {
            $(dependancies[key]).each(function(key2, value) {
                if(value == component && !$('#component_'+value).attr("checked"))
                {
                    $('#component_'+key).removeAttr("checked");
                    
                    checkDependeciesTo(key);
                    return;
                }
            });
        }
    }
}
function markDependecies(component, dependson)
{
    if(!dependancies[component])
    {
        dependancies[component] = new Array();
    }
    
    dependancies[component].push(dependson);
}
function indexOf(value, array)
{
    for(var key in array)
    {
        if(array[key] == value)
        {
            return key;
        }
    }
    
    return false;
}
function action_step1()
{
    
}
function action_step2()
{
    var valid = 0;
    $('#next').attr("disabled", "disabled");
    
    $('#check1').attr('src', '/install/img/loader.gif');
    $('#check2').attr('src', '/install/img/loader.gif');
    $('#check3').attr('src', '/install/img/loader.gif');
    $('#check4').attr('src', '/install/img/loader.gif');
    
    //check requirements
    jQuery.getJSON('/install.php', {action: 'checkphp'}, function(data) {
        if(data)
        {
            $('#check1').attr('src', '/install/img/ok-icon.png');
            valid++;
        }
        else
        {
            $('#check1').attr('src', '/install/img/fail-icon.png');
        }
        
        if(valid == 4)
        {
            $('#next').removeAttr("disabled");
        }
        else
        {
            $('#next').attr("disabled", "disabled");
        }
    });
    jQuery.getJSON('/install.php', {action: 'checkmysql'}, function(data) {
        if(data)
        {
            $('#check2').attr('src', '/install/img/ok-icon.png');
            valid++;
        }
        else
        {
            $('#check2').attr('src', '/install/img/fail-icon.png');
        }
        
        if(valid == 4)
        {
            $('#next').removeAttr("disabled");
        }
        else
        {
            $('#next').attr("disabled", "disabled");
        }
    });
    jQuery.getJSON('/install.php', {action: 'checkcurl'}, function(data) {
        if(data)
        {
            $('#check3').attr('src', '/install/img/ok-icon.png');
            valid++;
        }
        else
        {
            $('#check3').attr('src', '/install/img/fail-icon.png');
        }
        
        if(valid == 4)
        {
            $('#next').removeAttr("disabled");
        }
        else
        {
            $('#next').attr("disabled", "disabled");
        }
    });
    jQuery.getJSON('/install.php', {action: 'checkgdmod'}, function(data) {
        if(data)
        {
            $('#check4').attr('src', '/install/img/ok-icon.png');
            valid++;
        }
        else
        {
            $('#check4').attr('src', '/install/img/fail-icon.png');
        }
        
        if(valid == 4)
        {
            $('#next').removeAttr("disabled");
        }
        else
        {
            $('#next').attr("disabled", "disabled");
        }
    });
}
function action_step3()
{
    
}
function action_step4()
{
    $('#setting_auth').empty();
    
    $('.input_components').each(function(key, value) {
        if($(value).attr('checked'))
        {
            $('#setting_auth').append("<option value='"+$(value).val()+"'>"+$('#label_'+$(value).val()).text()+"</option>")
        }
    });
}
function action_step5()
{
    var components = new Array();
    $('#tobeinstalledcomponents').empty();
    $('#nottobeinstalledcomponents').empty();
    $('#appliedsettings').empty();
    
    $('.input_components').each(function(key, value) {
        if($(value).attr('checked'))
        {
            components.push($(value).val());
            
            $('#tobeinstalledcomponents').append("<li>"+$('#label_'+$(value).val()).text()+"</li>");
        }
        else
        {
            $('#nottobeinstalledcomponents').append("<li>"+$('#label_'+$(value).val()).text()+"</li>");
        }
    });
    
    finalData = {
        'components[]':     components,
        debug:              ($('#settings_debug').attr('checked'))? 1 : 0,
        authcomponent:      $($('#setting_auth')[0].options[$('#setting_auth')[0].selectedIndex]).val(),
        proxy:              $('#settings_proxy').attr('checked'),
        proxy_url:          $('#setting_proxyurl').val(),
        proxy_user:         $('#setting_proxyuser').val(),
        proxy_pass:         $('#setting_proxypass').val(),
        admin_needslogin:   $('#setting_needslogin').attr('checked'),
        admin_location:     $('#setting_adminlocation').val(),
        admin_login:        $('#setting_adminlogin').val(),
        admin_default:      $('#setting_needslogin').attr('checked'),
        dbhost:             $('#setting_dbhost').val(),
        dbdatabase:         $('#setting_dbdatabase').val(),
        dbusername:         $('#setting_dbuser').val(),
        dbpassword:         $('#setting_dbpass').val()
    };
    
    $('#appliedsettings').append("<li><label>Database Host:</label>"+finalData.dbhost+"</li>");
    $('#appliedsettings').append("<li><label>Database:</label>"+finalData.dbdatabase+"</li>");
    $('#appliedsettings').append("<li><label>Database Username:</label>"+finalData.dbusername+"</li>");
    $('#appliedsettings').append("<li><label>Database Password:</label>"+finalData.dbpassword+"</li>");
    
    $('#appliedsettings').append("<li><label>Debug:</label>"+((finalData.debug == 1)? 'on': 'off')+"</li>");
    $('#appliedsettings').append("<li><label>Authentication Component:</label>"+finalData.authcomponent+"</li>");
    $('#appliedsettings').append("<li><label>Proxy:</label>"+((finalData.proxy == 1)? 'on': 'off')+"</li>");
    
    if(finalData.proxy)
    {
        $('#appliedsettings').append("<li><label>Proxy URL:</label>"+finalData.proxy_url+"</li>");
        $('#appliedsettings').append("<li><label>Proxy User:</label>"+finalData.proxy_user+"</li>");
        $('#appliedsettings').append("<li><label>Proxy Password:</label>"+finalData.proxy_pass+"</li>");
    }
    
    $('#appliedsettings').append("<li><label>Admin Location:</label>"+finalData.admin_location+"</li>");
    $('#appliedsettings').append("<li><label>Admin Login:</label>"+finalData.admin_login+"</li>");
    $('#appliedsettings').append("<li><label>Need login for Admin:</label>"+((finalData.admin_needslogin == 1)? 'yes': 'no')+"</li>");
    $('#appliedsettings').append("<li><label>Default Admin pages:</label>"+((finalData.admin_default == 1)? 'yes': 'no')+"</li>");
}

function toggelProxyFields(checkbox) 
{
    if($(checkbox).attr('checked'))
    {
        $('#setting_proxyurl').removeAttr('disabled');
        $('#setting_proxyuser').removeAttr('disabled');
        $('#setting_proxypass').removeAttr('disabled');
    }
    else
    {
        $('#setting_proxyurl').attr('disabled', 'disabled');
        $('#setting_proxyuser').attr('disabled', 'disabled');
        $('#setting_proxypass').attr('disabled', 'disabled');
    }
}

function confirmDeletion(checkbox)
{
    if($(checkbox).attr('checked') && !confirm("Deleted componented are prementently removed and cannot be installed later on. Are you sure you want to delete them?"))
    {
        $(checkbox).removeAttr('checked');
    }
}