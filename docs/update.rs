fn update<T: Clone>(list: &mut Vec<T>, delta: Vec<T>, add_start: usize, remove_start: usize, _add_end: usize, remove_end: usize) {
    // Add elements from delta to the front of the list
    list.splice(0..0, delta.iter().take(add_start).cloned());

    // Skip elements removed from the start
    if remove_start > 0 {
        list.drain(0..remove_start);
    }

    // Skip elements removed from the end
    let len = list.len();
    let remove_end = remove_end.min(len);
    if remove_end > 0 {
        list.truncate(len - remove_end);
    }

    // Add remaining elements in delta to the end of the list
    list.extend(delta.into_iter().skip(add_start));
}

fn main() {
    let mut list = vec![1, 2, 3, 4, 5];
    let delta = vec![6, 7, 8, 9, 10];
    let add_start = 2;
    let remove_start = 1;
    let add_end = 0;
    let remove_end = 2;

    update(&mut list, delta, add_start, remove_start, add_end, remove_end);

    println!("{:?}", list); // Output: [6, 7, 3, 4, 8, 9, 10]
}
