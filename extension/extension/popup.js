
 function getCurrentTabUrl(callback) {
  var queryInfo = {
    active: true,
    currentWindow: true
  };

  chrome.tabs.query(queryInfo, (tabs) => {
    var tab = tabs[0];
    var url = tab.url;
    console.assert(typeof url == 'string', 'tab.url should be a string');
    callback(url);
  });
}



function injecttext(string, id) {
  var script = 'document.getElementById("' + id + '").value = "' + string + '";';
  chrome.tabs.executeScript({
    code: script
  });
}

function test(){
var app_id = "ngkjemfeajegloicpohncnenmehofjlj";

    var message = "300";
    chrome.runtime.sendMessage(app_id, message, function(result) {
        if (chrome.runtime.lastError) {
            // Handle error, e.g. app not installed
            console.warn('Error: ' + chrome.runtime.lastError.message);
        } else {
            // Handle success
            console.log('Reply from app: ', result);
        }
    });
}


document.addEventListener('DOMContentLoaded', () => {
  getCurrentTabUrl((url) => {
    link = document.getElementById('log');
    link.addEventListener('click', () => {
        test();
    });
  });
});


chrome.runtime.onMessageExternal.addListener(
  function(message, sender, sendResponse) {
    // TODO: Validate that sender.id is allowed to invoke the app!
    console.log('TODO: Do something with ' + message );
    if(message=="quentin@gmail.com"){
      console.log("Received id ");
        injecttext('quentin@gmail.com',"email");
        injecttext('p@ssw0rd',"pass");
    }
    sendResponse('Response received');
});

