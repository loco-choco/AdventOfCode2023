static class CodeExtractor{
  static Dictionary<string,int> conversion_table = new Dictionary<string,int>{
    {"0",0}, 
    {"zero",0}, 
    {"1",1}, 
    {"one",1}, 
    {"2",2}, 
    {"two",2}, 
    {"3",3}, 
    {"three",3}, 
    {"4",4}, 
    {"four",4}, 
    {"5",5}, 
    {"five",5}, 
    {"6",6}, 
    {"six",6}, 
    {"7",7}, 
    {"seven",7}, 
    {"8",8}, 
    {"eight",8}, 
    {"9",9}, 
    {"nine",9}, 
  };
  static void Main(string[] args){
    if(args.Length <= 0){
      Console.WriteLine("You need to provide the file name for decoding.");
      return;
    }
    int sum = 0;
    foreach(string line in File.ReadAllLines(args[0])){
      sum += DecodeString(line);
    }
    Console.WriteLine(sum);
  }
  static int DecodeString(string s){
    int first_number = 0;
    int last_number = 0;

    int first_number_index = s.Length;
    foreach((string key, int value) in conversion_table){
      int index = s.IndexOf(key); 
      if(index >= 0 && index < first_number_index){
        first_number_index = index;
	first_number = value;
      }
    }
    int last_number_index = -1;
    foreach((string key, int value) in conversion_table){
      int index = s.LastIndexOf(key); 
      if(index > last_number_index){
        last_number_index = index;
	last_number = value;
      }
    }

    return first_number * 10 + last_number;
  }
}


