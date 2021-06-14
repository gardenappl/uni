
class Cookie {
	constructor(name, value, expireDate, domain, path) {
		this.encodedName = name;
		this.encodedValue = value;
		this.expireDate = expireDate;
		this.domain = domain;
		this.path = path;
	}

	toString() {
		let result = this.encodedName;
		result += '=';
		result += this.encodedValue;
		if (this.expireDate)
			result += "; expires=" + this.expireDate;
		if (this.domain)
			result += "; domain=" + this.domain;
		if (this.path)
			result += "; path=" + this.path;

		return result;
	}

	createTableRow() {
		let row = document.createElement("tr");


		let td = document.createElement("td");
		td.className = "cookie-delete";
		let input = document.createElement("button");
		input.textContent = "\u{1F5D9}";
		input.addEventListener("click", handleClickDelete);

		td.appendChild(input);
		row.appendChild(td);


		td = document.createElement("td");
		td.className = "cookie-name";
		td.textContent = decodeURIComponent(this.encodedName);

		row.appendChild(td);


		td = document.createElement("td");
		td.className = "cookie-value";
		input = document.createElement("input");
		input.value = decodeURIComponent(this.encodedValue);

		td.appendChild(input);
		row.appendChild(td);


		td = document.createElement("td");
		td.className = "cookie-expire-date";
		if (this.expireDate !== undefined) {
			input = document.createElement("input");
			input.setAttribute("type", "date");
			
			td.appendChild(input);

			input = document.createElement("input");
			input.setAttribute("type", "time");
			input.setAttribute("step", "1");

			td.appendChild(input);
			row.appendChild(td);
		} else {
			input = document.createElement("button");
			input.addEventListener("click", allowExpireDateEdit);
			input.textContent = "Задати значення";

			td.appendChild(input);
			row.appendChild(td);
		}

		td = document.createElement("td");
		td.className = "cookie-domain";
		if (this.domain !== undefined) {
			input = document.createElement("input");
			input.placeholder = "За замовчуванням";
			input.value = this.domain;

			td.appendChild(input);
			row.appendChild(td);
		} else {
			input = document.createElement("button");
			input.textContent = "Задати значення";
			input.addEventListener("click", allowTextEdit);

			td.appendChild(input);
			row.appendChild(td);
		}


		td = document.createElement("td");
		td.className = "cookie-path";
		if (this.path !== undefined) {
			input = document.createElement("input");
			input.placeholder = "За замовчуванням";
			input.value = this.path;

			td.appendChild(input);
			row.appendChild(td);
		} else {
			input = document.createElement("button");
			input.textContent = "Задати значення";
			input.addEventListener("click", allowTextEdit);

			td.appendChild(input);
			row.appendChild(td);
		}


		return row;
	}
}

//Cookie Editor actions
class CookieEditorDelete {
	constructor(deletedCookie) {
		this.deletedCookie = deletedCookie;
	}

	applyToDocument() {
		this.deletedCookie.expireDate = new Date(1970, 1, 1);
		console.log(this.deletedCookie.toString());
		document.cookie = this.deletedCookie.toString();
	}
}

class CookieEditorOverwrite {
	constructor(newCookieString) {
		this.newCookieString = newCookieString;
	}

	applyToDocument() {
		document.cookie = this.newCookieString;
	}
}

class CookieEditorCreate {
	constructor(cookieName) {
		this.cookieName = cookieName;
	}

	applyToDocument() {
		document.cookie = this.cookieName + "=";
	}
}




const editorCookies = [];
let pendingActions = [];


window.onload = function() {
	//document.cookie = "hello=hi";
	populateCookieList();
	document.getElementById("cookie-table").addEventListener("change", handleTableEdit);
};


function addPendingAction(action) {
	pendingActions.push(action);
	let saveChangesButton = document.getElementById("save-changes");
	saveChangesButton.removeAttribute("disabled");
}

function populateCookieList() {
	let cookiesTable = document.getElementById("cookie-table");

	if (document.cookie.length == 0)
		return;

	let cookies = document.cookie.split(";");	
	for (let cookie of cookies) {
		let [name, value] = cookie.split("=");
		name = name.trimStart();
		let editorCookie = new Cookie(name, value);

		cookiesTable.appendChild(editorCookie.createTableRow());
		editorCookies.push(editorCookie);
	}
}

function allowExpireDateEdit(event) {
	let parent = event.target.parentElement;
	parent.textContent = ''; //clear children

	input = document.createElement("input");
	input.setAttribute("type", "date");

	parent.appendChild(input);

	input = document.createElement("input");
	input.setAttribute("type", "time");
	input.setAttribute("step", "1");

	parent.appendChild(input);

	pendingChanges = true;
}

function allowTextEdit(event) {
	let parent = event.target.parentElement;
	parent.textContent = ''; //clear children

	input = document.createElement("input");
	input.setAttribute("type", "text");

	parent.appendChild(input);

	pendingChanges = true;
}

function saveChanges() {
	for (let action of pendingActions)
		action.applyToDocument();

	pendingActions.length = 0; //clear array

	let saveChangesButton = document.getElementById("save-changes");
	saveChangesButton.setAttribute("disabled", true);
}


function alertCookies() {
	alert(document.cookie);
}

function handleTableEdit(event) {
	let element = event.target;
	let td = element.parentElement;
	let row = td.parentElement;

	let cookieIndex = Array.prototype.indexOf.call(row.parentElement.children, row) - 1;
	let cookie = editorCookies[cookieIndex];
	let noPendingAction = false;

	if (td.classList.contains("cookie-value")) {
		cookie.encodedValue = encodeURIComponent(element.value);

	} else if (td.classList.contains("cookie-expire-date")) {
		let date = td.children[0].value;
		let time = td.children[1].value;

		if (date.length == 0 && time.length == 0)
			cookie.expireDate = null;
		else if (date.length != 0 && time.length != 0)
			cookie.expireDate = dateAndTimeInputParse(date, time).toUTCString();
		else
			noPendingAction = true;
	} else if (td.classList.contains("cookie-path")) {
		cookie.path = encodeURI(element.value);

	} else if (td.classList.contains("cookie-domain")) {
		cookie.domain = encodeURI(element.value);

	}
	console.log(editorCookies[cookieIndex]);
	if (!noPendingAction)
		addPendingAction(new CookieEditorOverwrite(cookie));
}

function handleClickDelete(event) {
	let element = event.target;
	let td = element.parentElement;
	let row = td.parentElement;
	console.log(row);
	let name = row.getElementsByClassName("cookie-name")[0].textContent;
	name = encodeURIComponent(name);

	let rowIndex = Array.prototype.indexOf.call(row.parentElement.children, row);
	console.log(rowIndex);
	let cookieIndex = rowIndex - 1;

	row.parentElement.removeChild(row);
	addPendingAction(new CookieEditorDelete(editorCookies[cookieIndex]));
	editorCookies.splice(cookieIndex, cookieIndex + 1);
}


function createCookie() {
	let name = encodeURIComponent(prompt("Введіть ім'я cookie:"));
	if (name.length == 0 || name == "null" || !name) {
		alert("Ім'я cookie не може бути пустим");
		return;
	}
	addPendingAction(new CookieEditorCreate(name));
	let cookie = new Cookie(name, "");
	editorCookies.push(cookie);
	let row = cookie.createTableRow();
	let table = document.getElementById("cookie-table");
	table.appendChild(row);
}


//Helper functions
function dateAndTimeInputParse(dateValue, timeValue) {
	let [year, month, day] = dateValue.split('-');
	let [hours, minutes, seconds] = timeValue.split(':');
	return new Date(year, month, day, hours, minutes, seconds);
}

