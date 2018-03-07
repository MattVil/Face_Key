
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




document.addEventListener('DOMContentLoaded', () => {
  getCurrentTabUrl((url) => {
    link = document.getElementById('log');
    link.addEventListener('click', () => {
        injecttext('etudiant@gmail.com',"email");
        injecttext('p@ssw0rd',"pass");
    });
  });
});

