#!/bin/bash

# Address Book Shell Script
# File to store contacts
ADDRESS_BOOK="address_book.txt"

# Create address book file if it doesn't exist
touch "$ADDRESS_BOOK"

# Function to display menu
display_menu() {
    echo "==================================="
    echo "        ADDRESS BOOK MENU"
    echo "==================================="
    echo "1. Create New Address Book"
    echo "2. View All Contacts"
    echo "3. Insert New Contact"
    echo "4. Delete Contact"
    echo "5. Search Contact"
    echo "6. Modify Contact"
    echo "7. Exit"
    echo "==================================="
    echo -n "Enter your choice [1-7]: "
}

# Function to create new address book
create_address_book() {
    echo -n "Are you sure you want to create a new address book? (old data will be lost) [y/n]: "
    read confirmation
    if [ "$confirmation" = "y" ] || [ "$confirmation" = "Y" ]; then
        > "$ADDRESS_BOOK"
        echo "New address book created successfully!"
    else
        echo "Operation cancelled."
    fi
}


view_contacts() {
    if [ ! -s "$ADDRESS_BOOK" ]; then
        echo "Address book is empty!"
        return
    fi
    
    echo "==================================="
    echo "          ALL CONTACTS"
    echo "==================================="
    printf "%-3s %-20s %-15s %-30s %-15s\n" "ID" "Name" "Phone" "Email" "Address"
    echo "----------------------------------------------------------------------------------------"
    
    # Add line numbers and display
    nl -w 2 -s ". " "$ADDRESS_BOOK" | while read line; do
        echo "$line"
    done
}

# Function to insert new contact
insert_contact() {
    echo "Enter contact details:"
    
    # Input validation
    while true; do
        echo -n "Name: "
        read name
        if [ -n "$name" ]; then
            break
        else
            echo "Name cannot be empty!"
        fi
    done
    
    while true; do
        echo -n "Phone: "
        read phone
        if [ -n "$phone" ]; then
            break
        else
            echo "Phone cannot be empty!"
        fi
    done
    
    echo -n "Email: "
    read email
    
    echo -n "Address: "
    read address
    
    # Save to file
    echo "$name|$phone|$email|$address" >> "$ADDRESS_BOOK"
    echo "Contact added successfully!"
}

# Function to delete contact
delete_contact() {
    if [ ! -s "$ADDRESS_BOOK" ]; then
        echo "Address book is empty!"
        return
    fi
    
    view_contacts
    echo -n "Enter the ID of contact to delete: "
    read id
    
    # Check if ID is valid
    total_lines=$(wc -l < "$ADDRESS_BOOK")
    if [ "$id" -ge 1 ] && [ "$id" -le "$total_lines" ] 2>/dev/null; then
        # Create temporary file without the deleted line
        sed "${id}d" "$ADDRESS_BOOK" > temp.txt
        mv temp.txt "$ADDRESS_BOOK"
        echo "Contact deleted successfully!"
    else
        echo "Invalid ID!"
    fi
}

# Function to search contact
search_contact() {
    if [ ! -s "$ADDRESS_BOOK" ]; then
        echo "Address book is empty!"
        return
    fi
    
    echo -n "Enter name or phone to search: "
    read search_term
    
    if [ -z "$search_term" ]; then
        echo "Search term cannot be empty!"
        return
    fi
    
    echo "==================================="
    echo "         SEARCH RESULTS"
    echo "==================================="
    printf "%-3s %-20s %-15s %-30s %-15s\n" "ID" "Name" "Phone" "Email" "Address"
    echo "----------------------------------------------------------------------------------------"
    
    # Search and display with line numbers
    grep -i -n "$search_term" "$ADDRESS_BOOK" | while IFS=: read line_num line; do
        IFS='|' read -r name phone email address <<< "$line"
        printf "%-3s %-20s %-15s %-30s %-15s\n" "$line_num" "$name" "$phone" "$email" "$address"
    done
}

# Function to modify contact
modify_contact() {
    if [ ! -s "$ADDRESS_BOOK" ]; then
        echo "Address book is empty!"
        return
    fi
    
    view_contacts
    echo -n "Enter the ID of contact to modify: "
    read id
    
    # Check if ID is valid
    total_lines=$(wc -l < "$ADDRESS_BOOK")
    if [ "$id" -ge 1 ] && [ "$id" -le "$total_lines" ] 2>/dev/null; then
        # Get the current contact details
        current_contact=$(sed -n "${id}p" "$ADDRESS_BOOK")
        IFS='|' read -r current_name current_phone current_email current_address <<< "$current_contact"
        
        echo "Current details:"
        echo "Name: $current_name"
        echo "Phone: $current_phone"
        echo "Email: $current_email"
        echo "Address: $current_address"
        echo ""
        echo "Enter new details (press Enter to keep current value):"
        
        echo -n "Name [$current_name]: "
        read name
        name=${name:-$current_name}
        
        echo -n "Phone [$current_phone]: "
        read phone
        phone=${phone:-$current_phone}
        
        echo -n "Email [$current_email]: "
        read email
        email=${email:-$current_email}
        
        echo -n "Address [$current_address]: "
        read address
        address=${address:-$current_address}
        
        # Replace the line in file
        sed -i "${id}s/.*/$name|$phone|$email|$address/" "$ADDRESS_BOOK"
        echo "Contact modified successfully!"
    else
        echo "Invalid ID!"
    fi
}

# Main program loop
while true; do
    display_menu
    read choice
    
    case $choice in
        1)
            create_address_book
            ;;
        2)
            view_contacts
            ;;
        3)
            insert_contact
            ;;
        4)
            delete_contact
            ;;
        5)
            search_contact
            ;;
        6)
            modify_contact
            ;;
        7)
            echo "Thank you for using Address Book!"
            echo "Goodbye!"
            exit 0
            ;;
        *)
            echo "Invalid choice! Please enter a number between 1-7."
            ;;
    esac
    
    echo
    echo -n "Press Enter to continue..."
    read
    clear
done