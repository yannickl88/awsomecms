var currentstep = 1;
var dependancies = {};
var development = {};
var dependanciesChecked = new Array();
var disabled = false;
var stepsexec = new Array();
var valid = 0;
var authComponents = new Array();

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
    
    $('#progressInfo').text("");
    
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
        $('#next').text("Finish");
        $('#progressInfo').html("<img src='/install/img/loader.gif'/> Checking DB");
        $('#next').attr("disabled", "disabled");
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
    
    window.location = "#step_"+number;
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
function check(component)
{
    if($('#component_'+component).attr("checked"))
    {
        var install = true;
        
        //check if under development
        if(development[component] == true)
        {
            install = confirm("This component is under development, are you sure you want to install it?");
        }
        
        if(install)
        {
            dependanciesChecked = new Array();
            checkDependeciesFrom(component);
            
            dependanciesChecked = new Array();
            checkDependeciesTo(component);
        }
        else
        {
            $('#component_'+component).removeAttr("checked");
        }
    }
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
function markUnderDev(component)
{
    development[component] = true;
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
function checkItem(id, check, total)
{
    $(id).attr('src', '/install/img/loader.gif');
    
    //check requirements
    jQuery.getJSON('/install.php', {action: check}, function(data) {
        if(data)
        {
            $(id).attr('src', '/install/img/ok-icon.png');
            valid++;
        }
        else
        {
            $(id).attr('src', '/install/img/fail-icon.png');
        }
        
        if(valid == total)
        {
            $('#next').removeAttr("disabled");
            $('#progressInfo').text("");
        }
        else
        {
            $('#next').attr("disabled", "disabled");
            $('#progressInfo').html("<img src='/install/img/loader.gif'/> Checking Requirements");
        }
    });
}

function checkDB()
{
    if(finalData.dbdatabase == "")
    {
        $('#progressInfo').html("<span class='error'><img src='/install/img/fail-icon.png' alt='fail' /> No Database filled in</span>");
    }
    else
    {
        //check requirements
        jQuery.getJSON('/install.php', {action: "checkdbconnection", host: finalData.dbhost, db: finalData.dbdatabase, user: finalData.dbusername, pass: finalData.dbpassword}, function(data) {
            if(data.success)
            {
                $('#next').removeAttr("disabled");
                $('#progressInfo').text("");
            }
            else
            {
                $('#progressInfo').html("<span class='error'><img src='/install/img/fail-icon.png' alt='fail' /> "+data.error+"</span>");
            }
        });
    }
}

function action_step2()
{
    valid = 0;
    $('#next').attr("disabled", "disabled");
    
    checkItem('#check1', 'checkphp', 4);
    checkItem('#check2', 'checkmysql', 4);
    checkItem('#check3', 'checkgdmod', 4);
    checkItem('#check4', 'checkapache', 4);
}
function action_step3()
{
    
}
function action_step4()
{
    $('#setting_auth').empty();
    
    $('.input_components').each(function(key, value) {
        if($(value).attr('checked') && in_array($(value).val(), authComponents))
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
        admin_default:      $('#setting_default').attr('checked'),
        dbhost:             $('#setting_dbhost').val(),
        dbdatabase:         $('#setting_dbdatabase').val(),
        dbusername:         $('#setting_dbuser').val(),
        dbpassword:         $('#setting_dbpass').val()
    };
    
    $('#appliedsettings').append("<li><label>Database Host:</label><span>"+finalData.dbhost+"</span></li>");
    $('#appliedsettings').append("<li><label>Database:</label><span>"+finalData.dbdatabase+"</span></li>");
    $('#appliedsettings').append("<li><label>Database Username:</label><span>"+finalData.dbusername+"</span></li>");
    $('#appliedsettings').append("<li><label>Database Password:</label><span>"+finalData.dbpassword+"</span></li>");
    
    $('#appliedsettings').append("<li><label>Debug:</label><span>"+((finalData.debug == 1)? 'on': 'off')+"</span></li>");
    $('#appliedsettings').append("<li><label>Authentication Component:</label><span>"+finalData.authcomponent+"</span></li>");
    $('#appliedsettings').append("<li><label>Proxy:</label><span>"+((finalData.proxy == 1)? 'on': 'off')+"</span></li>");
    
    if(finalData.proxy)
    {
        $('#appliedsettings').append("<li><label>Proxy URL:</label><span>"+finalData.proxy_url+"</span></li>");
        $('#appliedsettings').append("<li><label>Proxy User:</label><span>"+finalData.proxy_user+"</span></li>");
        $('#appliedsettings').append("<li><label>Proxy Password:</label><span>"+finalData.proxy_pass+"</span></li>");
    }
    
    $('#appliedsettings').append("<li><label>Admin Location:</label><span>"+finalData.admin_location+"</span></li>");
    $('#appliedsettings').append("<li><label>Admin Login:</label><span>"+finalData.admin_login+"</span></li>");
    $('#appliedsettings').append("<li><label>Need login for Admin:</label><span>"+((finalData.admin_needslogin == 1)? 'yes': 'no')+"</span></li>");
    $('#appliedsettings').append("<li><label>Default Admin pages:</label><span>"+((finalData.admin_default == 1)? 'yes': 'no')+"</span></li>");
    
    checkDB();
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

function addAuthComponent(componentName)
{
    authComponents.push(componentName);
}

function in_array(needel, array)
{
    var inArray = false;
    
    $(array).each(function(key, value) {
        if(needel == value)
        {
            inArray = true
            return;
        }
    });
    
    return inArray;
}