/*This is currently hardcoded for now before full integration is complete.*/
const courses = {
  COMP1701: {
    title: "COMP1701 - Introduction to Problem Solving and Programming",
    description: "This course provides an introduction to problem solving in the context of computer programming. The course emphasizes fundamental algorithmic solutions and implementation of those solutions using a practical programming language. Topics include data representation, program control, file handling and elementary data structures.",
    prerequisites: "None",
  },
  COMP1633: {
    title: "COMP1633 - Introduction to Computer Science II",
    description: "Introduction to object-oriented analysis and design, programming using an object-oriented language, and implementation of linked data structures. Issues of modularity, software design, and programming style will be emphasized.",
    prerequisites: "COMP1701",
  },
  COMP2631: {
    title: "COMP2631 - Information Structures I",
    description: "Data structures important to computer science will be studied, including trees, graphs and hash tables. Searching and sorting techniques will be emphasized. Associated algorithms and their time and space efficiency will also be studied.",
    prerequisites: "COMP1633",
  },
  COMP2633: {
    title: "COMP2633 - Foundations of Software Engineering",
    description: "Introduction to software development problems and to the processes and methods used to address them. Software life cycles models. Software process improvement. Goals and methods for requirements analysis and specification, software design, implementation, integration and testing of software. An introduction to one or more informal methods (focusing on object-oriented approaches) currently in wide spread use.",
    prerequisites: "COMP2631",
  },
/* This course exists for us to easily hardcode and duplicate multiple courses if we need them.
  COMP1633: {
    title: "COMP1633 - Title",
    description: "desc",
    prerequisites: "None",
  },
  */
};