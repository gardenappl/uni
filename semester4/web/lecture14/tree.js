
//data is a multi-dimensional array. Element 0 of each sub-array is its name.
const data = ["Всесвіт",
	["Молочний Шлях", 
		["Сонце", ["Венера"], ["Земля", ["Місяць"]], ["Марс"]],
		["Чорна діра 124124"],	
		["Cіріус", ["Планета Сіріус-1"], ["Планета Сіріус-2"]]
	],
	["Галактика Андромеди",
		["Зірка Епсілон-А", ["Планета Епсілон-А1"], ["Планета Епсілон-А2"], ["Планета Епсілон-А3"]],
		["Чорна діра 14124"]
	]
];

//Each DOM element from the tree has an ID in the form item-<index1>-<index2>-...
//For example, Земля has id="item-1-1-2"

//Also, each item can also be accessed using a Path array, in the form [<index1>, <index2>]

function getItemFromId(elementId) {
	let splitId = elementId.split('-');
	let currentItem = data;
	for (let i = 1; i < splitId.length; i++) {
		currentItem = currentItem[splitId[i]];
	}
	return currentItem;
}

function getItemFromPath(path) {
	let currentItem = data;
	for (let index of path)
		currentItem = currentItem[index];
	return currentItem;
}

function pathToId(path) {
	let result = "item";
	for (let index of path)
		result += '-' + index;
	return result;
}

function idToPath(id) {
	console.log("converting " + id);
	let splitId = id.split('-');
	let path = [];
	for (let i = 1; i < splitId.length; i++) {
		path.push(splitId[i]);
	}
	console.log("result: " + path);
	return path;
}

function createExpandedElementForItem(path, item) {
	let element = createElementForItem(path, item);
	appendChildren(path, item, element);

	return element;
}

function createElementForItem(path, item) {
	let element = document.createElement("div");
	let id = pathToId(path);
	element.setAttribute("id", id);
	element.classList.add("level" + path.length);
	element.classList.add("item-element");

	let div = document.createElement("div");
	div.classList.add("level" + path.length);
	div.classList.add("item-name");
	div.textContent = item[0];
	element.appendChild(div);

	if (item.length > 1)
		div.classList.add("expandable");

	return element;
}

function appendChildren(path, item, element) {
	for (let i = 1; i < item.length; i++) {
		let childPath = path.slice(); //copy
		childPath.push(i);
		console.log("Appending" + path);

		let child = createElementForItem(childPath, item[i]);

		element.appendChild(child);
	}
}

function toggleTreeElement(element) {
	let id = element.getAttribute("id");
	let item = getItemFromId(id);
	let nameDiv = element.children[0];

	if (nameDiv.classList.contains("expanded")) {
		while (element.children.length > 1) {
			console.log("Removing " + element.lastChild);
			element.removeChild(element.lastChild);
		}
		nameDiv.classList.remove("expanded");
		nameDiv.classList.add("expandable");
	} else if (item.length > 1) {
		nameDiv.classList.remove("expandable");
		nameDiv.classList.add("expanded");
		appendChildren(idToPath(id), item, element);
	}
}

function handleTreeClick(event) {
	let element = event.target;
	console.log(event);
	if (element.classList.contains("item-name"))
		toggleTreeElement(element.parentElement);
}


window.onload = function() {
	console.log(getItemFromId("item-1-1-2"));
	let container = document.getElementById("tree-container");

	container.addEventListener("click", handleTreeClick);
	console.log(data);

	container.appendChild(createElementForItem([], data));
};
