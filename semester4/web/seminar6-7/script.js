let formatCost = function(cost) {
	let hrn = Math.floor(cost);
	let kopiykas = Math.floor((cost - hrn) * 100);
	if (kopiykas < 10)
		kopiykas = "0" + kopiykas;

	return hrn + "," + kopiykas + " грн.";
}


let cart = new Map();
let bookCosts = new Map();
bookCosts.set("cybernetics", 10.5);
bookCosts.set("cybernetics2", 21);
bookCosts.set("not-cybernetics", 50.1);
bookCosts.set("the-result", 13);
bookCosts.set("web", 30);


DeliveryType = {
	Home: 1,
	PostOffice: 2
};

class DeliveryInfo {
	constructor(deliveryType, presentWrap) {
		this.deliveryType = deliveryType;
		this.presentWrap = presentWrap;
	}
}

class OrderItem {
	constructor(name, amount, price, delivery) {
		this.name = name;
		this.amount = amount;
		this.price = price;
		this.deliveryInfo = delivery;
	}

	getCost() {
		return this.price * this.amount;
	}

	getDeliveryCost() {
		let cost = this.getCost();
		let deliveryCost = 0;

		if (this.deliveryInfo.deliveryType == DeliveryType.Home)
			deliveryCost = cost * 0.15;
		else
			deliveryCost = cost * 0.05;

		if (this.deliveryInfo.presentWrap)
			deliveryCost *= 1.1;

		return deliveryCost;
	}

	getTotalCost() {
		return this.getCost() + this.getDeliveryCost();
	}
}

function updateCart() {
	console.log(bookCosts);
	console.log("called");
	let bookCostField = document.querySelector("input[id='total-amount'");
	const bookDropdown = document.querySelector("select[id='book']");

	const selectedBook = bookDropdown.options[bookDropdown.selectedIndex];

	console.log(selectedBook.value);
	if (selectedBook.value.length > 0) {
		const amountEntered = document.querySelector("input[id='book-amount']").value;
		const amount = Number(amountEntered);

		console.log(amount);
		console.log(amountEntered);

		if (amount == 0) {
			cart.delete(selectedBook.value);
		} else {
			const presentWrap = document.querySelector("input[id='present-wrap']").checked;
			const deliveryType = document.querySelector("input[id='home-delivery']").checked ?
				DeliveryType.Home :
				DeliveryType.PostOffice;

			const deliveryInfo = new DeliveryInfo(deliveryType, presentWrap);
			const name = document.querySelector("option[value='" + selectedBook.value + "']").innerHTML;

			const newOrder = new OrderItem(
				name,
				amount,
				bookCosts.get(selectedBook.value),
				deliveryInfo
			);

			cart.set(selectedBook.value, newOrder);
			console.log(cart);
		}
		
	}
}

function onAmountEntered() {
	const bookDropdown = document.querySelector("select[id='book']");
	const selectedBook = bookDropdown.options[bookDropdown.selectedIndex];

	updateCart();

	if (!cart.has(selectedBook.value))
		return;

	const amountEntered = document.querySelector("input[id='book-amount']").value;
	const amount = Number(amountEntered);

	const totalPrice = cart.get(selectedBook.value).getCost();
	document.querySelector("input[id='total-cost']").value = formatCost(totalPrice);
}

function onBookSelected() {
	const bookDropdown = document.querySelector("select[id='book']");
	const selectedBook = bookDropdown.options[bookDropdown.selectedIndex].getAttribute("value");

	let amount = 0;
	let deliveryType = DeliveryType.PostOffice;
	let presentWrap = false;
	let totalPrice = 0;

	if (cart.has(selectedBook)) {
		let orderInfo = cart.get(selectedBook);

		amount = orderInfo.amount;
		deliveryType = orderInfo.deliveryInfo.deliveryType;
		presentWrap = orderInfo.deliveryInfo.presentWrap;
		const cost = bookCosts.get(selectedBook);
		totalPrice = cost * amount;

	}

	document.querySelector("input[id='book-amount']").value = amount;
	document.querySelector("input[id='total-cost']").value = totalPrice;
	document.querySelector("input[id='present-wrap']").checked = presentWrap;
	document.querySelector("input[id='post-office']").checked = deliveryType == DeliveryType.PostOffice;
	document.querySelector("input[id='home-delivery']").checked = deliveryType == DeliveryType.Home;
};

function calcTotal() {
	let totalField = document.querySelector("input[id='final-cost']");
	
	let cost = 0;
	for (let item of cart.values()) {
		cost += item.getTotalCost();
	}
	totalField.value = formatCost(cost);
}

function clearCart() {
	cart = new Map();

	document.querySelector("input[id='book-amount']").value = 0;
	document.querySelector("input[id='total-cost']").value = 0;
	document.querySelector("input[id='present-wrap']").checked = false;
	document.querySelector("input[id='post-office']").checked = true;
	document.querySelector("input[id='home-delivery']").checked = false;
	console.log("cleared");
}


function submit() {
	let formResultDiv = document.getElementById("form-result");
	//Прибираємо елементи
	while (formResultDiv.firstChild)
		formResultDiv.removeChild(formResultDiv.lastChild);

	let formResult;

	const nameField = document.getElementById("user-real-name");
	const addressField = document.getElementById("user-address");

	if (nameField.value.length == 0) {
		formResult = document.createElement("div");
		formResult.className = "invalid-warning";

		if (addressField.value.length > 0)
			formResult.innerHTML = "Будь ласка, введіть своє ім'я";
		else
			formResult.innerHTML = "Будь ласка, введіть своє ім'я та адресу доставки";
	} else if (addressField.value.length == 0) {
		formResult = document.createElement("div");
		formResult.className = "invalid-warning";

		formResult.innerHTML = "Будь ласка, введіть адресу доставки";
	} else if (cart.size == 0) {
		formResult = document.createElement("div");
		formResult.className = "invalid-warning";

		formResult.textContent = "Ваш кошик пустий.";
	} else {
		formResult = document.createElement("table");
		formResult.className = "result";

		let tableRow = document.createElement("tr");

		let col = document.createElement("th");
		col.textContent = "Товари";
		col.setAttribute("colspan", 2);
		tableRow.appendChild(col);

		col = document.createElement("th");
		col.textContent = "Вартість книг";
		tableRow.appendChild(col);

		col = document.createElement("th");
		col.textContent = "Доставка";
		tableRow.appendChild(col);

		col = document.createElement("th");
		col.textContent = "Вартість доставки";
		tableRow.appendChild(col);

		col = document.createElement("th");
		col.textContent = "Загальна вартість";
		tableRow.appendChild(col);

		formResult.appendChild(tableRow);	
		
		let finalCost = 0;
		let finalCostDelivery = 0;
		let finalCostTotal = 0;

		for (let [bookId, orderItem] of cart) {
			tableRow = document.createElement("tr");

			col = document.createElement("td");
			col.className = "book-name";
			col.textContent = document.querySelector("option[value='" + bookId + "']")
				.textContent;
			tableRow.appendChild(col);


			col = document.createElement("td");
			col.className = "book-amount";
			col.textContent = orderItem.amount + " шт.";
			tableRow.appendChild(col);


			let cost = orderItem.getCost();
			finalCost += cost;
			
			col = document.createElement("td");
			col.textContent = formatCost(cost);
			tableRow.appendChild(col);


			let deliveryMethod = "";
			if (orderItem.deliveryInfo.deliveryType == DeliveryType.Home)
				deliveryMethod += "Додому";
			else
				deliveryMethod += "На пошту";

			if(orderItem.deliveryInfo.presentWrap)
				deliveryMethod += ", подарунок";

			col = document.createElement("td");
			col.className = "delivery-method";
			col.textContent = deliveryMethod;
			tableRow.appendChild(col);


			let costDelivery = orderItem.getDeliveryCost();
			finalCostDelivery += costDelivery;

			col = document.createElement("td");
			col.className = "cost";
			col.textContent = formatCost(costDelivery);
			tableRow.appendChild(col);

			
			let costTotal = orderItem.getTotalCost();
			finalCostTotal += costTotal;

			col = document.createElement("td");
			col.className = "cost total";
			col.textContent = formatCost(costTotal);
			tableRow.appendChild(col);

			formResult.appendChild(tableRow);
		}
		tableRow = document.createElement("tr");

		col = document.createElement("td");
		col.setAttribute("colspan", 2);
		col.textContent = "Всього";
		tableRow.appendChild(col);

		col = document.createElement("td");
		col.className = "cost";
		col.textContent = formatCost(finalCost);
		tableRow.appendChild(col);

		tableRow.appendChild(document.createElement("td"));

		col = document.createElement("td");
		col.className = "cost";
		col.textContent = formatCost(finalCostDelivery);
		tableRow.appendChild(col);

		col = document.createElement("td");
		col.className = "cost";
		col.textContent = formatCost(finalCostTotal);
		tableRow.appendChild(col);


		formResult.appendChild(tableRow);
	}


	formResultDiv.appendChild(formResult);
}

