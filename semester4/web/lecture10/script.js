function palindromeClear() {
	document.getElementById("palindrome-result").value = "";
}

function palindromeCheck() {
	const value = document.getElementById("palindrome-input").value;
	const cleanValue = value.toLowerCase().replace(/\s/g, '');
	//console.log(cleanValue);

	
	let isPalindrome = true;
	for (var i = 0; i < cleanValue.length / 2; i++) {
		if (cleanValue[i] !== cleanValue[cleanValue.length - 1 - i])
			isPalindrome = false;
	}

	const resultString = isPalindrome ?
		"является палиндромом" :
		"не является палиндромом";
	document.getElementById("palindrome-result").value = resultString;
}




function wordSearch() {
	const text = document.getElementById("search-input").value;
	const query = document.getElementById("search-query").value;

	let wordCount = 0;

	const words = text.split(/[\W\p]/);
	for (const word of words) {
		if (word.toLowerCase() == query.toLowerCase())
			wordCount++;
	}

	document.getElementById("search-count").value = wordCount;
}

function wordClear() {
	document.getElementById("search-count").value = "";
	document.getElementById("search-query").value = "";
}




function examCalculate() {
	const value = document.getElementById("exam-input").value;

	const grades = value.split(' ');
	let totalCount = 0;
	let gradeCounts = [];
	for (var i = 2; i <= 5; i++)
		gradeCounts[i] = 0;

	for (const grade of grades) {
		let isMark = true;
		if (grade == "2") {
			gradeCounts[2]++;
		} else if (grade == "3") {
			gradeCounts[3]++;
		} else if (grade == "4") {
			gradeCounts[4]++;
		} else if (grade == "5") {
			gradeCounts[5]++;
		} else {
			isMark = false;
		}

		if (isMark)
			totalCount++;
	}

	document.getElementById("exam-count").value = totalCount;

	let average = 0;
	for (var i = 2; i <= 5; i++) {
		average += i * gradeCounts[i];
		document.getElementById("grade" + i).value = gradeCounts[i];
	}
	average /= totalCount;
	document.getElementById("exam-average").value = average;

}


function examClear() {
	const inputs = document.querySelectorAll(".exam input");
	for(const input of inputs)
		input.value = "";
}
	
