import 'package:flutter_test/flutter_test.dart';
import 'package:allarme_camper/main.dart';

void main() {
  testWidgets('Check MyApp', (WidgetTester tester) async {
    await tester.pumpWidget(const MyApp());
    // Cerca il titolo che appare nella AppBar
    expect(find.text('Allarme Camper'), findsOneWidget);
  });
}