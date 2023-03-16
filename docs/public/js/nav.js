let isOpen = false;

function toggleNav() {
	isOpen = !isOpen;
	if (isOpen) {
		document.querySelector("#navbar nav").classList.add("open");
	} else {
		document.querySelector("#navbar nav").classList.remove("open");
	}
}
