import csv
from cs50 import SQL


def main():
    db = SQL("sqlite:///prophecy.db")

    students_data_csv = "students.csv"

    with open(students_data_csv, "r") as file:
        reader = csv.DictReader(file)

        for row in reader:
            student_name = row["student_name"]
            student_id = create_student(db, student_name)
            print(f"Saved student_id {student_id}.")

            house = row["house"]
            head = row["head"]
            existing_house = get_house(db, house)
            if existing_house:
                house_id = existing_house[0]["id"]
            else:
                house_id = create_house(db, house, head)
                print(f"Saved house_id {house_id}.")

            assignment_id = create_assignment(db, student_id, house_id)
            print(f"Saved assignment_id {assignment_id}.")

            print(f"Assignment {assignment_id}: {student_name} to {house}.")


def create_student(db, student_name):
    query = "INSERT INTO students (student_name) VALUES (?)"
    return db.execute(query, student_name)


def create_house(db, house, head):
    query = "INSERT INTO houses (house, head) VALUES (?, ?)"
    return db.execute(query, house, head)


def create_assignment(db, student_id, house_id):
    query = "INSERT INTO assignments (student_id, house_id) VALUES (?, ?)"
    return db.execute(query, student_id, house_id)


def get_house(db, house):
    query = "SELECT id FROM houses WHERE house = ?"
    return db.execute(query, house)


if __name__ == "__main__":
    main()
