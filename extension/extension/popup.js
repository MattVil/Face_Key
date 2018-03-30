
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

function splitString(stringToSplit, separator) {
  return arrayOfStrings = stringToSplit.split(separator);
}

function injecttext(string, id) {
  var script = 'document.getElementById("' + id + '").value = "' + string + '";';
  chrome.tabs.executeScript({
    code: script
  });
}

function messagepassing(message){
var app_id = "ngkjemfeajegloicpohncnenmehofjlj";

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
        messagepassing("300");
    });
  });
});


chrome.runtime.onMessageExternal.addListener(
  function(message, sender, sendResponse) {
    // TODO: Validate that sender.id is allowed to invoke the app!
    console.log('TODO: Do something with ' + message );
    if(message=="301"){
      console.log("client is ok i will send domain name to app ");
      var site;
        chrome.tabs.getSelected(null,function(tab) {//get current tab without any selectors
          site = tab.url;
          console.log("Le site est : " + site);
          messagepassing("302;"+"google.com");
      });
    }
    else if(message=="999"){
      console.log("Erreur dans la connexion");
      alert("erreur");
    }
    var slash = "/";
    var ptvirgule = ";";
    var splitmsg = splitString(message, ptvirgule);
    var code = splitmsg[0];
    if(code=="305"){
      console.log("I just get log ids : " + splitmsg[1]);
      var ids = splitString(splitmsg[1],slash);
      injecttext(ids[0],"email");
      injecttext(ids[1],"pass");
    }

    sendResponse('Response received');
});

