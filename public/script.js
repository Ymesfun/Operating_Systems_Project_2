/*This handles our dropdowns in the course-list section*/
const groups = document.querySelectorAll('.group');

groups.forEach(group => {
  const title = group.querySelector('.group-title');
  const list = group.querySelector('.course-list');

  title.addEventListener('click', () => {
    // Toggle active state
    group.classList.toggle('active');

    // Expand or collapse smoothly
    if (group.classList.contains('active')) {
      list.style.maxHeight = list.scrollHeight + "px";
    } else {
      list.style.maxHeight = null;
    }
  });
});
/* This function handles loading course details on click, instead of making 50+ html files. This scales better when we integrate with our db.
   Event gets passed in for safety, as the global 'event' is depracated and non functional in some browsers..
*/
function loadCourse(courseCode) {
  /*it hurts hardcoding things, but by default we're just reading off of courses.js*/
  const course = courses[courseCode];
  const content = document.querySelector(".content");
    /* Emergency error handling in case an entry doesn't exist. */
  if (!course) {
    content.innerHTML = "<p>Course not found.</p>";
    return;
  }
  /*Gets the header/title/prereq for modification*/
  const header = document.querySelector("header h1");
  const pageTitle = document.querySelector("title");
  const prereqHTML = generatePrerequisites(course.prerequisites); /*generates a hyperlink to the prereqs if they exist, if not returns none.*/

  // Update document title and header breadcrumb
  pageTitle.textContent = `SelfCOMP > Courses > ${courseCode}`;
  header.textContent = `SelfCOMP > Courses > ${courseCode}`;
  
  
  content.innerHTML = `
    <section class="course-detail">
      <h2>${course.title}</h2>
      <div class="course-section">
        <h3>Description</h3>
        <p>${course.description}</p>
      </div>
    <div class="course-section">
      <h3>Prerequisites</h3>
      <p>
        ${prereqHTML === "None" ? "Prerequisites: None" : `Prerequisites: ${prereqHTML} with C- or higher`}
      </p>
    </div>
      <div class="course-actions">
        <button class="btn course-btn">Comments</button>
        <button class="btn course-btn">Files</button>
      </div>
    </section>
  `;
}
document.querySelectorAll('.course-list a').forEach(link => {
  link.addEventListener('click', function(e) {
    e.preventDefault();
    const courseCode = this.getAttribute('href').substring(1); // remove #
    loadCourse(courseCode);
  });
});
/* On page load, check if a hash exists, and use it to load your previously open page. Makes refreshes non-destructive.*/
window.addEventListener('DOMContentLoaded', () => {
  const hash = window.location.hash.slice(1); /* remove the #*/
  if (hash) {
    loadCourse(hash);
  }
});

/* Function to generate hyperlinked prerequisites dynamically*/
function generatePrerequisites(prereq) {
  if (!prereq || prereq.toLowerCase() === "none") return "None";

  /* Split multiple prerequisites if needed (assuming comma-separated)*/
  const prereqArray = prereq.split(",").map(p => p.trim());

  /* Map each prerequisite to a hyperlink if it exists in courses.js*/
  const linkedPrereqs = prereqArray.map(code => {
    if (courses[code]) {
      return `<a href="#${code}" class="course-link" onclick="loadCourse('${code}')">${code}</a>`;    
    } else {
      return code; /* fallback if course code not found*/
    }
  });

  return linkedPrereqs.join(", ");
}
